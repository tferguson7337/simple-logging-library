#include <StreamLogger.h>

namespace SLL
{
    /// Private Helper Methods \\\

    // Initialize Stream.
    void StreamLogger::InitializeStream( )
    {
        // Ensure the stream has a buffer (stdout).
        if ( !mStream.rdbuf( ) )
        {
            mStream.set_rdbuf(std::wcout.rdbuf( ));
        }

        if ( !mStream.good( ) || !mStream.rdbuf( ) )
        {
            throw std::runtime_error(
                std::string(__FUNCTION__" - Failed to initialize stream: ") +
                "good == " +
                std::to_string(mStream.good( )) +
                ", buffer == " +
                (mStream.rdbuf( ) ? std::string("valid") : std::string("null"))
            );
        }
    }
    
    // Restore Stream to Good State.
    bool StreamLogger::RestoreStream( )
    {
        // Static trackers for logging failed restore attempts to stderr.
        const static size_t FAIL_FREQ_START = 1;
        static size_t failTotalCount = 0;
        static size_t failTicker = 0;
        static size_t failFreq = FAIL_FREQ_START;

        try
        {
            // Clear stream state and attempt to flush.
            mStream.clear( );
            InitializeStream( );
            mStream.flush( );
        }
        catch ( const std::exception& e )
        {
            // Best effort - log recovery failure to stderr.
            // Less frequent reports as more failed attempts occur.
            if ( (failTicker++ % failFreq) == 0 )
            {
                failTicker = 1;
                failFreq++;

                std::fprintf(
                    stderr,
                    "\n\n   %s - Failed to restore stream from bad state (attempt %zu): %s",
                    __FUNCTION__,
                    ++failTotalCount,
                    e.what( )
                );
            }

            return false;
        }
        
        // Successfully recovered - reset fail trackers.
        failTotalCount = 0;
        failTicker = 0;
        failFreq = FAIL_FREQ_START;

        return mStream.good( );
    }

    // Flush Buffer Contents To Stream.
    void StreamLogger::Flush(const VerbosityLevel& lvl)
    {
        if ( !mStream.good( ) )
        {
            return;
        }

        // Flush messages to file periodically, or if the message is likely important.
        if ( (mFlushCounter++ % mFlushInterval) == 0 || lvl >= VerbosityLevel::WARN )
        {
            mStream.flush( );
        }
    }

    // Log Prefixes to Stream.
    template <class T, typename>
    void StreamLogger::LogPrefixes(const VerbosityLevel& lvl, const std::thread::id& tid)
    {
        std::vector<std::unique_ptr<T[ ]>> prefixes;

        if ( !mStream.good( ) )
        {
            throw std::runtime_error(__FUNCTION__" - Stream in bad state, cannot write prefix strings.");
        }

        try
        {
            prefixes = BuildMessagePrefixes<T>(lvl, tid);
        }
        catch ( const std::exception& )
        {
            // Best effort, nothing to do.
            return;
        }

        for ( auto& p : prefixes )
        {
            if ( !mStream.good( ) )
            {
                throw std::runtime_error(__FUNCTION__" - Stream in bad state, cannot write message prefix to stream.");
            }

            mStream << p.get( );
        }
    }

    // Log User Message To File.
    template <class T, typename>
    void StreamLogger::LogMessage(const T* pFormat, va_list pArgs)
    {
        std::unique_ptr<T[ ]> message;

        if ( !mStream.good( ) )
        {
            throw std::runtime_error(__FUNCTION__" - Stream in bad state, cannot write message to stream.");
        }

        if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid format string (null).");
        }

        try
        {
            message = BuildFormattedMessage<T>(pFormat, pArgs);
        }
        catch ( const std::exception& )
        {
            // Best effort, nothing to do.
            return;
        }

        mStream << message.get( ) << L"\n";
    }

    /// Constructors \\\

    // ConfigPackage Constructor [C]
    StreamLogger::StreamLogger(const ConfigPackage& config) :
        LoggerBase(config),
        mStream(std::wcout.rdbuf( ))
    { }

    // ConfigPackage Constructor [M]
    StreamLogger::StreamLogger(ConfigPackage&& config) noexcept :
        LoggerBase(std::move(config)),
        mStream(std::wcout.rdbuf( ))
    { }

    // Move Constructor
    StreamLogger::StreamLogger(StreamLogger&& src) noexcept :
        LoggerBase(std::move(src)),
        mStream(std::wcout.rdbuf( ))
    {
        *this = std::move(src);
    }

    /// Destructor \\\

    // Virtual Destructor
    StreamLogger::~StreamLogger( )
    {
        if ( mStream.good( ) )
        {
            try
            {
                mStream.flush( );
            }
            catch ( const std::exception& )
            {
                // Best attempt - nothing more to be done.
            }
        }
    }

    /// Assignment Overload \\\

    StreamLogger& StreamLogger::operator=(StreamLogger&& src)
    {
        if ( this == &src )
        {
            throw std::invalid_argument(__FUNCTION__" - Attempted self-assignment.");
        }

        return *this;
    }

    /// Public Methods \\\

    template <class T, typename>
    bool StreamLogger::Log(const VerbosityLevel& lvl, const T* pFormat, va_list pArgs)
    {
        // Ensure verbosity level is valid.
        if ( lvl < VerbosityLevel::BEGIN || lvl >= VerbosityLevel::MAX )
        {
            throw std::invalid_argument(
                __FUNCTION__" - Invalid verbosity level argument (" +
                std::to_string(static_cast<VerbosityLevelType>(lvl)) +
                ")."
            );
        }

        // Ensure we have a valid format string.
        if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__ " - Invalid format string (nullptr).");
        }

        // Ensure the arg list is valid.
        if ( !pArgs )
        {
            throw std::invalid_argument(__FUNCTION__ " - Invalid argument list (nullptr).");
        }

        // Don't log if message level is below the configured verbosity threshold.
        if ( lvl < GetConfig( ).GetVerbosityThreshold( ) )
        {
            return true;
        }

        // Check if the file stream is still open and in a good state, attempt to recover if not.
        if ( !mStream.good( ) && !RestoreStream( ) )
        {
            return false;
        }

        // Log message prefix strings.
        try
        {
            LogPrefixes<T>(lvl, std::this_thread::get_id( ));
        }
        catch ( const std::exception& )
        {
            // Best effort - we'll attempt to restore to a good state next log.
            mStream.setstate(std::ios_base::badbit);
            return mStream.good( );
        }

        // If file stream state is still good, log user message.
        try
        {
            LogMessage<T>(pFormat, pArgs);
        }
        catch ( const std::exception& )
        {
            // Best effort - we'll attempt to restore to a good state next log.
            mStream.setstate(std::ios_base::badbit);
            return mStream.good( );
        }

        // Flush messages to file periodically, or if the message is likely important.
        Flush(lvl);

        return mStream.good( );
    }

    template <class T, typename>
    bool StreamLogger::Log(const VerbosityLevel& lvl, const T* pFormat, ...)
    {
        va_list pArgs;

        // Ensure verbosity level is valid.
        if ( lvl < VerbosityLevel::BEGIN || lvl >= VerbosityLevel::MAX )
        {
            throw std::invalid_argument(
                __FUNCTION__" - Invalid verbosity level argument (" +
                std::to_string(static_cast<VerbosityLevelType>(lvl)) +
                ")."
            );
        }

        // Ensure we have a valid format string.
        if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__ " - Invalid format string (nullptr).");
        }

        // Ensure the arg list is valid.
        if ( !pArgs )
        {
            throw std::invalid_argument(__FUNCTION__ " - Invalid argument list (nullptr).");
        }

        // Don't log if message level is below the configured verbosity threshold.
        if ( lvl < GetConfig( ).GetVerbosityThreshold( ) )
        {
            return true;
        }

        // Check if the file stream is still open and in a good state, attempt to recover if not.
        if ( !mStream.good( ) && !RestoreStream( ) )
        {
            return false;
        }

        // Log message prefix strings.
        try
        {
            LogPrefixes<T>(lvl, std::this_thread::get_id( ));
        }
        catch ( const std::exception& )
        {
            // Best effort - we'll attempt to restore to a good state next log.
            mStream.setstate(std::ios_base::badbit);
            return mStream.good( );
        }

        // If file stream state is still good, log user message.
        try
        {
            va_start(pArgs, pFormat);
            LogMessage<T>(pFormat, pArgs);
            va_end(pArgs);
        }
        catch ( const std::exception& )
        {
            // Best effort - we'll attempt to restore to a good state next log.
            va_end(pArgs);
            mStream.setstate(std::ios_base::badbit);
            return mStream.good( );
        }

        // Flush messages to file periodically, or if the message is likely important.
        Flush(lvl);

        return mStream.good( );
    }

}