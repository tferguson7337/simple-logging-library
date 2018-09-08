#include <SyncLogger.h>

#include <LoggerFactory.h>

namespace SLL
{
    /// Private Worker Methods \\\

    // Logging loop for the worker thread.
    void SyncLogger::WorkerLogLoop( )
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
    void SyncLogger::WaitForMsgs( )
    {
        // Helper lambda for wait condition.
        static const auto waitLambda = [this] ( ) -> bool
        {
            return this->WaitPredicate( );
        };

        std::unique_lock<std::mutex> lock(mMsgQueueMutex);

        mMsgCV.wait(lock, waitLambda);
    }

    // Worker thread's wait condition method.
    bool SyncLogger::WaitPredicate( ) const noexcept
    {
        // Wake up if we have work or if we need to terminate.
        return !mMsgQueue.empty( ) || mTerminate;
    }

    // Worker thread's termination condition method.
    bool SyncLogger::TerminatePredicate( )
    {
        return mTerminate && (mMsgQueueSize == 0);
    }

    // Public method helper for enqueuing new LogMessages.
    void SyncLogger::PushMsg(LogMessage&& msg)
    {
        std::lock_guard<std::mutex> lg(mMsgQueueMutex);

        mMsgQueue.push(std::forward<LogMessage>(msg));

        mMsgQueueSize++;

        if ( !mWorkerThread.joinable( ) )
        {
            mWorkerThread = std::thread(&SyncLogger::WorkerLogLoop, this);
        }
    }

    // Worker thread's obtain-next-message method.
    std::queue<SyncLogger::LogMessage> SyncLogger::GetQueuedMsgs( )
    {
        std::queue<LogMessage> msgs;

        std::lock_guard<std::mutex> lock(mMsgQueueMutex);
        
        msgs.swap(mMsgQueue);

        mMsgQueueSize = mMsgQueue.size( );
        
        return msgs;
    }

    // Main part of worker thread's work-flow.
    // Attempts to log queued message using the owned logger object.
    void SyncLogger::LogMsgs(std::queue<LogMessage>&& msgs)
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
                success = mpLogger->Log(msg.GetVerbosityLevel( ), msg.GetThreadID( ), L"%s", msg.GetString( ));
            }
            catch ( const std::exception& e )
            {
                try
                {
                    static const char* errFormat = __FUNCTION__" - Failed to log message: exception \"%s\", msg \"%ls\".";
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
    SyncLogger::SyncLogger(const ConfigPackage& config) :
        LoggerBase(ConfigPackage( )),
        mOptionMask(config.GetOptionFlags( )),
        mpLogger(nullptr),
        mSuccessCounter(0),
        mFailCounter(0)
    {
        // Disable LogSynchronous option when calling BuildLogger to avoid infinite recursion.
        ConfigPackage cp = config;
        cp.Disable(OptionFlag::LogSynchronous);

        // Get logger object via BuildLogger.
        mpLogger = BuildLogger(std::move(cp));
    }

    // Multiple-ConfigPackage Constructor [C]
    SyncLogger::SyncLogger(const ConfigPackage& stdOutConfig, const ConfigPackage& fileConfig) :
        LoggerBase(ConfigPackage( )),
        mOptionMask(stdOutConfig.GetOptionFlags( ) | fileConfig.GetOptionFlags( )),
        mpLogger(nullptr),
        mSuccessCounter(0),
        mFailCounter(0)
    {
        // Disable LogSynchronous option when calling BuildLogger to avoid infinite recursion.
        ConfigPackage sCP = stdOutConfig;
        ConfigPackage fCP = fileConfig;
        sCP.Disable(OptionFlag::LogSynchronous);
        fCP.Disable(OptionFlag::LogSynchronous);

        // Get logger object via BuildLogger.  
        mpLogger = BuildLogger(sCP, fCP);
    }

    /// Destructor \\\

    SyncLogger::~SyncLogger( )
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
    bool SyncLogger::Log(const VerbosityLevel& lvl, const char* pFormat, ...)
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
    bool SyncLogger::Log(const VerbosityLevel& lvl, const wchar_t* pFormat, ...)
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
    bool SyncLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const char* pFormat, ...)
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
    bool SyncLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const wchar_t* pFormat, ...)
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
    bool SyncLogger::Log(const VerbosityLevel& lvl, const char* pFormat, va_list pArgs)
    {
        std::unique_ptr<wchar_t[ ]> str;

        if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid format argument (null).");
        }

        // Build the log message.
        str = LoggerBase::BuildFormattedMessage<wchar_t>(StringUtil::ConvertAndCopy<wchar_t>(pFormat).get( ), pArgs);

        // Push the message into the queue.
        PushMsg(LogMessage(lvl, std::move(str), std::this_thread::get_id( )));

        return true;
    }

    // Submit log message to stream(s) (va_list, wide).
    bool SyncLogger::Log(const VerbosityLevel& lvl, const wchar_t* pFormat, va_list pArgs)
    {
        std::unique_ptr<wchar_t[ ]> str;

        if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid format argument (null).");
        }

        // Build the log message.
        str = LoggerBase::BuildFormattedMessage<wchar_t>(pFormat, pArgs);

        // Push the message into the queue.
        PushMsg(LogMessage(lvl, std::move(str), std::this_thread::get_id( )));

        return true;
    }

    // Submit log message to stream(s) (va_list, explicit thread ID).
    bool SyncLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const char* pFormat, va_list pArgs)
    {
        std::unique_ptr<wchar_t[ ]> str;

        if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid format argument (null).");
        }

        // Build the log message.
        str = LoggerBase::BuildFormattedMessage<wchar_t>(StringUtil::ConvertAndCopy<wchar_t>(pFormat).get( ), pArgs);

        // Push the message into the queue.
        PushMsg(LogMessage(lvl, std::move(str), tid));

        return true;
    }

    bool SyncLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const wchar_t* pFormat, va_list pArgs)
    {
        std::unique_ptr<wchar_t[ ]> str;

        if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid format argument (null).");
        }

        // Build the log message.
        str = LoggerBase::BuildFormattedMessage<wchar_t>(pFormat, pArgs);

        // Push the message into the queue.
        PushMsg(LogMessage(lvl, std::move(str), tid));

        return true;
    }
}