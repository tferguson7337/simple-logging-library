#include <FileLogger.h>

#include <cstdarg>

namespace SLL
{
    /// Private Helper Methods \\\

    // Initialize file stream by opening file specified in configuration package.
    void FileLogger::InitializeFileStream( )
    {
        static const std::string commonThrowStr(__FUNCTION__" - Failed to initialize file stream: ");

        const ConfigPackage& config = GetConfig( );

        // We require a filename from the config package to target.
        if ( config.GetFile( ).empty( ) )
        {
            throw std::logic_error(commonThrowStr + "No filename provided.");
        }

        try
        {
            // Attempt to open the file - append to file - write in binary mode.
            mFileStream.open(config.GetFile( ), std::ios_base::app | std::ios_base::binary | std::ios_base::out);
        }
        catch ( const std::exception& e )
        {
            throw std::runtime_error(commonThrowStr + e.what( ));
        }

        // See if we successfully opened the file and the stream is in a good state.
        if ( !(mFileStream.good( ) && mFileStream.is_open( )) )
        {
            throw std::runtime_error(
                commonThrowStr +
                "good == " +
                std::to_string(mFileStream.good( )) +
                ", is_open == " +
                std::to_string(mFileStream.is_open( ))
            );
        }
    }

    // Attempt to recover file stream from bad state by reinitializing.
    bool FileLogger::RestoreFileStream( )
    {
        // Static trackers for logging failed restore attempts to stderr.
        const static size_t FAIL_FREQ_START = 3;
        static size_t failTotalCount = 0;
        static size_t failTicker = 0;
        static size_t failFreq = FAIL_FREQ_START;

        try
        {
            // Clear stream state, close file stream, attempt to reopen.
            mFileStream.clear( );
            mFileStream.close( );
            InitializeFileStream( );
        }
        catch ( const std::exception& e )
        {
            // Best effort - log recovery failure to stderr.
            // Less frequent reports as more failed attempts occur.
            if ( (failTicker % failFreq) == 0 )
            {
                failTicker = 1;
                failFreq++;

                std::fprintf(
                    stderr,
                    "\n\n %s - Failed to restore file stream from bad state (attempt %zu): %s",
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

        return true;
    }

    // Build and log prefix strings as part of user's message to file.
    template <class T, typename>
    void FileLogger::LogPrefixes(const VerbosityLevel& lvl, const std::thread::id& tid)
    {
        std::vector<std::unique_ptr<T[ ]>> prefixes;

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
            try
            {
                mFileStream << p.get( );
            }
            catch ( const std::exception& )
            {
                // Best effort, attempt to restore file stream next go.
                mFileStream.setstate(std::ios_base::badbit);
            }
        }
    }

    // Build and log user's message to file.
    template <class T, typename>
    void FileLogger::LogMessage(const T* pFormat, va_list pArgs)
    {
        std::unique_ptr<T[ ]> message;

        try
        {
            message = BuildFormattedMessage<T>(pFormat, pArgs);
        }
        catch ( const std::exception& )
        {
            // Best effort, nothing to do.
            return;
        }

        try
        {
            mFileStream << message.get( ) << std::endl;
        }
        catch ( const std::exception& )
        {
            // Best effort, attempt to restore file stream next go.
            mFileStream.setstate(std::ios_base::badbit);
        }
    }

    /// Constructors \\\
            
    // ConfigPackage Constructor [C]
    FileLogger::FileLogger(const ConfigPackage& config) :
        LoggerBase(config)
    {
        InitializeFileStream( );
    }

    // ConfigPackage Constructor [M]
    FileLogger::FileLogger(ConfigPackage&& config) :
        LoggerBase(std::move(config))
    {
        InitializeFileStream( );
    }

    // Move Constructor
    FileLogger::FileLogger(FileLogger&& src) noexcept :
        LoggerBase(std::move(src)),
        mFileStream(std::move(src.mFileStream))
    { }

    /// Destructor \\\

    // Virtual Destructor
    FileLogger::~FileLogger( )
    {
        if ( mFileStream.good( ) && mFileStream.is_open( ) )
        {
            try
            {
                mFileStream.flush( );
            }
            catch ( const std::exception& )
            {
                // Best attempt - nothing more to be done.
            }
        }
    }

    /// Assignment Overload \\\

    // Move Assignment
    FileLogger& FileLogger::operator=(FileLogger&& src)
    {
        if ( this == &src )
        {
            throw std::invalid_argument(__FUNCTION__" - Attempted self-assignment.");
        }

        LoggerBase::operator=(std::move(src));
        mFileStream = std::move(src.mFileStream);

        return *this;
    }

    /// Public Method \\\

    // Flush buffer contents to file.
    void FileLogger::Flush( )
    {
        if ( mFileStream.good( ) && mFileStream.is_open( ) )
        {
            mFileStream.flush( );
        }
    }

    // Log formatted message to file.
    template <class T, typename>
    bool FileLogger::Log(const VerbosityLevel& lvl, const T* pFormat, ...)
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

        // Don't log if message level is below the configured verbosity threshold.
        if ( lvl < GetConfig( ).GetVerbosityThreshold( ) )
        {
            return true;
        }

        // Check if the file stream is still open and in a good state, attempt to recover if not.
        if ( !(mFileStream.is_open( ) && mFileStream.good( )) && !RestoreFileStream( ) )
        {
            return false;
        }

        // Log message prefix strings.
        LogPrefixes<T>(lvl, std::this_thread::get_id( ));

        // If file stream state is still good, log user message.
        if ( mFileStream.good( ) )
        {
            va_list pArgs;
            va_start(pArgs, pFormat);
            LogMessage<T>(pFormat, pArgs);
            va_end(pArgs);
        }
        
        // If the message is important enough, attempt to flush contents to file now.
        if ( lvl >= VerbosityLevel::WARN )
        {
            Flush( );
        }

        return mFileStream.good( );
    }

    /// Explicit Template Instantiations \\\

    template bool FileLogger::Log<char>(const VerbosityLevel&, const char*, ...);
    template bool FileLogger::Log<wchar_t>(const VerbosityLevel&, const wchar_t*, ...);
}