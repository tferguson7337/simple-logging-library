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
        while ( !mTerminate )
        {
            // Wait for more log messages to queue-up.
            // - Note: We won't actually sleep if the message queue isn't empty.
            WaitForMsgs( );

            // See if we should terminate after waking up.
            if ( mTerminate )
            {
                break;
            }

            // Ensure the logger didn't somehow get nulled.
            if ( !mpLogger )
            {
                throw std::runtime_error(__FUNCTION__" - mpLogger somehow became null during worker thread loop.");
            }

            // Log the next queued message.
            LogNextMsg( );
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

        std::unique_lock<std::mutex> ul(mMsgQueueLock);

        mMsgCV.wait(ul, waitLambda);
    }

    // Worker thread's wait condition method.
    bool SyncLogger::WaitPredicate( ) const noexcept
    {
        // Wake up if we have work or if we need to terminate.
        return !mMsgQueue.empty( ) || mTerminate;
    }

    // Public method helper for enqueuing new LogMessages.
    void SyncLogger::PushMsg(LogMessage&& msg)
    {
        std::lock_guard<std::mutex> lg(mMsgQueueLock);

        mMsgQueue.push(std::forward<LogMessage>(msg));
    }

    // Worker thread's obtain-next-message method.
    SyncLogger::LogMessage SyncLogger::PopMsg( )
    {
        std::lock_guard<std::mutex> lg(mMsgQueueLock);

        LogMessage msg = std::move(mMsgQueue.front( ));
        mMsgQueue.pop( );

        return msg;
    }

    // Main part of worker thread's work-flow.
    // Attempts to log queued message using the owned logger object.
    void SyncLogger::LogNextMsg( )
    {
        LogMessage msg(PopMsg( ));

        bool success = false;

        try
        {
            // Log the queued message, capture success/failure.
            success = mpLogger->Log(msg.GetVerbosityLevel( ), L"%s", msg.GetString( ));
        }
        catch ( const std::exception& e )
        {
            try
            {
                // Try to log that we failed with exception string and original message.
                mpLogger->Log(
                    VerbosityLevel::WARN,
                    L"%ls - Failed to log message: exception \"%ls\", msg \"%s\".",
                    __FUNCTION__,
                    e.what( ),
                    msg.GetString( )
                );
            }
            catch ( const std::exception& )
            {
                // Best effort - nothing to do here aside from incrementing failure count.
                mFailCounter++;
            }
        }

        // Record success/failure of log with counter.
        if ( success )
        {
            mSuccessCounter++;
        }
        else
        {
            mFailCounter++;
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
        // Disable LogSynchronous option to avoid infinite recursion.
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
        // Disable LogSynchronous option to avoid infinite recursion.
        ConfigPackage sCP = stdOutConfig;
        ConfigPackage fCP = fileConfig;
        sCP.Disable(OptionFlag::LogSynchronous);
        fCP.Disable(OptionFlag::LogSynchronous);

        // Get logger object via BuildLogger.  
        mpLogger = BuildLogger(stdOutConfig, fileConfig);
    }

    /// Destructor \\\

    SyncLogger::~SyncLogger( )
    {
        if ( mWorkerThread.joinable( ) )
        {
            mTerminate = true;
            mWorkerThread.join( );
        }

        if ( mpLogger )
        {
            try
            {
                mpLogger->Log(
                    VerbosityLevel::INFO,
                    L"%ls - successful logs = %zu, failed logs = %zu.\r\n",
                    __FUNCTION__,
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

    template <class T, typename>
    bool SyncLogger::Log(const VerbosityLevel& lvl, const T* pFormat, ...)
    {
        bool ret = false;
        va_list pArgs;
        va_start(pArgs, pFormat);

        try
        {
            ret = Log(lvl, pFormat, pArgs);
        }
        catch ( const std::exception& e )
        {
            va_end(pArgs);
            throw;
        }

        va_end(pArgs);
        return ret;
    }

    template <class T, typename>
    bool SyncLogger::Log(const VerbosityLevel& lvl, const T* pFormat, va_list pArgs)
    {
        std::unique_ptr<wchar_t[ ]> str;
        bool ret = false;

        if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid format argument (null).");
        }

        if constexpr ( std::is_same_v<T, char> )
        {
            str = LoggerBase::BuildFormattedMessage<wchar_t>(StringUtil::ConvertAndCopy<wchar_t>(pFormat).get( ), pArgs);
        }
        else
        {
            str = LoggerBase::BuildFormattedMessage<wchar_t>(pFormat.get( ), pArgs);
        }

        PushMsg(LogMessage(lvl, std::move(str)));

        return true;
    }
}