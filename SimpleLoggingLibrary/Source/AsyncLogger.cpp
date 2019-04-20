#include <AsyncLogger.h>

#include <LoggerFactory.h>

#include <CCStringUtil.h>

namespace SLL
{
	using Base = CC::StringUtil::NumberConversion::Base;
	using ReturnType = CC::StringUtil::ReturnType;

    /// Private Worker Methods \\\

    // Logging loop for the worker thread.
    void AsyncLogger::WorkerLogLoop( ) const
    {
        // Ensure we have a valid logger at thread start.
        if ( !mpLogger )
        {
            throw std::runtime_error(__FUNCTION__" - mpLogger was null at worker thread start.");
        }

        // Loop until we get signaled to terminate.
        while ( !TerminatePredicate( ) )
        {
            // Wait for more log messages to queue-up.
            // - Note: We won't actually sleep if the message queue isn't empty.
            WaitForMsgs( );

            // See if we should terminate after waking up.
            if ( TerminatePredicate( ) )
            {
                break;
            }

            // Ensure the logger didn't somehow get nulled.
            if ( !mpLogger )
            {
                throw std::runtime_error(__FUNCTION__" - mpLogger somehow became null during worker thread loop.");
            }

            // Log the next batch of queued messages.
            LogMsgs(GetQueuedMsgs( ));
        }
    }

    // Worker thread's wait method.
    void AsyncLogger::WaitForMsgs( ) const
    {
        std::unique_lock<std::mutex> lock(mMsgQueueMutex);

        mMsgCV.wait(lock, [this] ( ) -> bool
        {
            return this->WaitPredicate( );
        });
    }

    // Worker thread's wait condition method.
    bool AsyncLogger::WaitPredicate( ) const noexcept
    {
        // Wake up if we have work or if we need to terminate.
        return !mMsgQueue.empty( ) || mTerminate;
    }

    // Worker thread's termination condition method.
    bool AsyncLogger::TerminatePredicate( ) const
    {
        return mTerminate && (mMsgQueueSize == 0);
    }

    // Public method helper for enqueuing new LogMessages.
    void AsyncLogger::PushMsg(LogMessage&& msg) const
    {
        std::lock_guard<std::mutex> lg(mMsgQueueMutex);

        mMsgQueue.push(std::forward<LogMessage>(msg));

        mMsgQueueSize++;

        if ( !mWorkerThread.joinable( ) )
        {
            mWorkerThread = std::thread(&AsyncLogger::WorkerLogLoop, this);
        }

        mMsgCV.notify_one( );
    }

    // Worker thread's obtain-next-message method.
    std::queue<AsyncLogger::LogMessage> AsyncLogger::GetQueuedMsgs( ) const
    {
        std::queue<LogMessage> msgs;

        std::lock_guard<std::mutex> lock(mMsgQueueMutex);
        
        msgs.swap(mMsgQueue);

        mMsgQueueSize = mMsgQueue.size( );
        
        return msgs;
    }

    // Main part of worker thread's work-flow.
    // Attempts to log queued message using the owned logger object.
    void AsyncLogger::LogMsgs(std::queue<LogMessage>&& msgs) const
    {
        bool success = false;

        while ( !msgs.empty( ) )
        {
            // Pop message from the local LogMessage queue arg.
            LogMessage msg(std::move(msgs.front( )));
            msgs.pop( );

            try
            {
                // Log the queued message, capture success/failure.
                success = mpLogger->Log(msg.GetVerbosityLevel( ), msg.GetThreadID( ), UTF16_LITERAL_STR("%ls"), msg.GetString( ));
            }
            catch ( const std::exception& e )
            {
                try
                {
                    static const utf8* errFormat = __FUNCTION__" - Failed to log message: exception \"%s\", msg \"%ls\".";
                    // Try to log that we failed with exception string and original message.
                    mpLogger->Log(VerbosityLevel::WARN, errFormat, e.what( ), msg.GetString( ));
                }
                catch ( const std::exception& )
                {
                    // Best effort - nothing to do here aside from incrementing failure count.
                    mFailCounter++;
                }
            }

            // Record success/failure of log with counter.
            // We attempt to log these stats upon destruction.
            if ( success )
            {
                mSuccessCounter++;
            }
            else
            {
                mFailCounter++;
            }
        }
    }

    /// Constructors \\\

    // Single-ConfigPackage Constructor [C]
    AsyncLogger::AsyncLogger(const ConfigPackage& config) :
        LoggerBase(ConfigPackage( )),
        mOptionMask(config.GetOptionFlags( )),
        mpLogger(nullptr),
        mSuccessCounter(0),
        mFailCounter(0)
    {
        // Disable LogAsynchronous option when calling BuildLogger to avoid infinite recursion.
        ConfigPackage cp = config;
        cp.Disable(OptionFlag::LogAsynchronous);

        // Get logger object via BuildLogger.
        mpLogger = BuildLogger(std::move(cp));
    }

    // Multiple-ConfigPackage Constructor [C]
    AsyncLogger::AsyncLogger(const ConfigPackage& stdOutConfig, const ConfigPackage& fileConfig) :
        LoggerBase(ConfigPackage( )),
        mOptionMask(stdOutConfig.GetOptionFlags( ) | fileConfig.GetOptionFlags( )),
        mpLogger(nullptr),
        mSuccessCounter(0),
        mFailCounter(0)
    {
        // Disable LogAsynchronous option when calling BuildLogger to avoid infinite recursion.
        ConfigPackage sCP = stdOutConfig;
        ConfigPackage fCP = fileConfig;
        sCP.Disable(OptionFlag::LogAsynchronous);
        fCP.Disable(OptionFlag::LogAsynchronous);

        // Get logger object via BuildLogger.  
        mpLogger = BuildLogger(sCP, fCP);
    }

    /// Destructor \\\

    AsyncLogger::~AsyncLogger( )
    {
        // Check if worker thread is running.
        if ( mWorkerThread.joinable( ) )
        {
            // Signal the worker to terminate and wait.
            mTerminate = true;
            mWorkerThread.join( );
        }

        if ( mpLogger )
        {
            // Attempt to log one last message - report statistics.
            try
            {
                mpLogger->Log(
                    VerbosityLevel::INFO,
                    __FUNCTION__" - successful logs = %zu, failed logs = %zu.\r\n",
                    mSuccessCounter,
                    mFailCounter
                );
            }
            catch ( ... )
            {
                // Best effort - nothing else to do.
            }
        }
    }

    /// Public Methods \\\

    // Submit log message to stream(s) (variadic arguments, narrow).
    bool AsyncLogger::Log(const VerbosityLevel& lvl, const utf8* pFormat, ...) const
    {
        bool ret = false;
        va_list pArgs;
        va_start(pArgs, pFormat);

        try
        {
            ret = Log(lvl, std::this_thread::get_id( ), pFormat, pArgs);
        }
        catch ( const std::exception& )
        {
            va_end(pArgs);
            throw;
        }

        va_end(pArgs);
        return ret;
    }

    // Submit log message to stream(s) (variadic arguments, wide).
    bool AsyncLogger::Log(const VerbosityLevel& lvl, const utf16* pFormat, ...) const
    {
        bool ret = false;
        va_list pArgs;
        va_start(pArgs, pFormat);

        try
        {
            ret = Log(lvl, std::this_thread::get_id( ), pFormat, pArgs);
        }
        catch ( const std::exception& )
        {
            va_end(pArgs);
            throw;
        }

        va_end(pArgs);
        return ret;
    }

    // Submit log message to stream(s) (variadic arguments, narrow, explicit thread ID).
    bool AsyncLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const utf8* pFormat, ...) const
    {
        bool ret = false;
        va_list pArgs;
        va_start(pArgs, pFormat);

        try
        {
            ret = Log(lvl, tid, pFormat, pArgs);
        }
        catch ( const std::exception& )
        {
            va_end(pArgs);
            throw;
        }

        va_end(pArgs);
        return ret;
    }

    // Submit log message to stream(s) (variadic arguments, wide, explicit thread ID).
    bool AsyncLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const utf16* pFormat, ...) const
    {
        bool ret = false;
        va_list pArgs;
        va_start(pArgs, pFormat);

        try
        {
            ret = Log(lvl, tid, pFormat, pArgs);
        }
        catch ( const std::exception& )
        {
            va_end(pArgs);
            throw;
        }

        va_end(pArgs);
        return ret;
    }

    // Submit log message to stream(s) (va_list, narrow).
    bool AsyncLogger::Log(const VerbosityLevel& lvl, const utf8* pFormat, va_list pArgs) const
    {
        std::unique_ptr<utf16[ ]> str;

        if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid format argument (null).");
        }

        // Build the log message.
        str = LoggerBase::BuildFormattedMessage<utf16>(CC::StringUtil::UTFConversion<ReturnType::SmartCString, utf16, utf8>(pFormat).get( ), pArgs);

        // Push the message into the queue.
        PushMsg(LogMessage(lvl, std::move(str), std::this_thread::get_id( )));

        return true;
    }

    // Submit log message to stream(s) (va_list, wide).
    bool AsyncLogger::Log(const VerbosityLevel& lvl, const utf16* pFormat, va_list pArgs) const
    {
        std::unique_ptr<utf16[ ]> str;

        if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid format argument (null).");
        }

        // Build the log message.
        str = LoggerBase::BuildFormattedMessage<utf16>(pFormat, pArgs);

        // Push the message into the queue.
        PushMsg(LogMessage(lvl, std::move(str), std::this_thread::get_id( )));

        return true;
    }

    // Submit log message to stream(s) (va_list, explicit thread ID).
    bool AsyncLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const utf8* pFormat, va_list pArgs) const
    {
        std::unique_ptr<utf16[ ]> str;

        if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid format argument (null).");
        }

        // Build the log message.
		str = LoggerBase::BuildFormattedMessage<utf16>(CC::StringUtil::UTFConversion<ReturnType::SmartCString, utf16, utf8>(pFormat).get( ), pArgs);

        // Push the message into the queue.
        PushMsg(LogMessage(lvl, std::move(str), tid));

        return true;
    }

    bool AsyncLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const utf16* pFormat, va_list pArgs) const
    {
        std::unique_ptr<utf16[ ]> str;

        if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid format argument (null).");
        }

        // Build the log message.
        str = LoggerBase::BuildFormattedMessage<utf16>(pFormat, pArgs);

        // Push the message into the queue.
        PushMsg(LogMessage(lvl, std::move(str), tid));

        return true;
    }
}