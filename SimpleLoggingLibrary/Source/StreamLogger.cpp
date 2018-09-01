// Class Declaration
#include <StreamLogger.h>

// For enabling console color logging
// using Window's Virtual Terminal
#include <WindowsConsoleHelper.h>

// STL synchronization
#include <atomic>
#include <mutex>

namespace SLL
{
    /// Static Private Data Member Initialization \\\

    // Color sequences for Windows 10 (Threshold 2 and beyond) console color output.
    // Defined in https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences (Text Formatting)
    template <class StreamType>
    const std::vector<std::basic_string<char>> StreamLogger<StreamType>::mColorSequencesA
    {
        "\x1b[30m",  // BLACK
        "\x1b[31m",  // RED
        "\x1b[32m",  // GREEN
        "\x1b[33m",  // YELLOW
        "\x1b[34m",  // BLUE
        "\x1b[35m",  // MAGENTA
        "\x1b[36m",  // CYAN
        "\x1b[37m",  // WHITE

        "\x1b[90m",  // BRIGHT_BLACK
        "\x1b[91m",  // BRIGHT_RED
        "\x1b[92m",  // BRIGHT_GREEN
        "\x1b[93m",  // BRIGHT_YELLOW
        "\x1b[94m",  // BRIGHT_BLUE
        "\x1b[95m",  // BRIGHT_MAGENTA
        "\x1b[96m",  // BRIGHT_CYAN
        "\x1b[97m",  // BRIGHT_WHITE

        "\x1b[39m",  // DEFAULT
    };

    template <class StreamType>
    const std::vector<std::basic_string<wchar_t>> StreamLogger<StreamType>::mColorSequencesW
    {
        L"\x1b[30m",  // BLACK
        L"\x1b[31m",  // RED
        L"\x1b[32m",  // GREEN
        L"\x1b[33m",  // YELLOW
        L"\x1b[34m",  // BLUE
        L"\x1b[35m",  // MAGENTA
        L"\x1b[36m",  // CYAN
        L"\x1b[37m",  // WHITE

        L"\x1b[90m",  // BRIGHT_BLACK
        L"\x1b[91m",  // BRIGHT_RED
        L"\x1b[92m",  // BRIGHT_GREEN
        L"\x1b[93m",  // BRIGHT_YELLOW
        L"\x1b[94m",  // BRIGHT_BLUE
        L"\x1b[95m",  // BRIGHT_MAGENTA
        L"\x1b[96m",  // BRIGHT_CYAN
        L"\x1b[97m",  // BRIGHT_WHITE

        L"\x1b[39m",  // DEFAULT
    };

    /// Private Helper Methods - Specialization \\\

    // Return whether or not stream is in good state (stdout).
    template <>
    bool StreamLogger<StdOutStream>::IsStreamGood( ) const noexcept
    {
        return mStream.good( ) && mStream.rdbuf( );
    }

    // Return whether or not stream is in good state (file).
    template <>
    bool StreamLogger<FileStream>::IsStreamGood( ) const noexcept
    {
        return mStream.good( ) && mStream.is_open( );
    }

    // Initialize Stream (stdout)
    template <>
    void StreamLogger<StdOutStream>::InitializeStream( )
    {
        // Ensure the stream has a buffer (stdout).
        if ( !mStream.rdbuf( ) )
        {
            mStream.set_rdbuf(mpWideStreamBuffer);

            if ( !mStream.good( ) )
            {
                mStream.clear( );
            }
        }

        if ( GetConfig( ).OptionEnabled(OptionFlag::LogInColor) )
        {
            if ( !WindowsConsoleHelper::Initialize( ) )
            {
                GetConfig( ).Disable(OptionFlag::LogInColor);
            }
        }

        if ( !IsStreamGood( ) )
        {
            throw std::runtime_error(
                std::string(__FUNCTION__" - Failed to initialize stream: ") +
                "good == " +
                std::to_string(mStream.good( )) +
                ", buffer == " +
                StringUtil::UnsignedToString<char>(StringUtil::ConversionType::Hexidecimal, mStream.rdbuf( ))
            );
        }
    }

    // Initialize Stream (file).
    template <>
    void StreamLogger<FileStream>::InitializeStream( )
    {
        static const std::string commonThrowStr(__FUNCTION__" - Failed to initialize file stream: ");

        const ConfigPackage& config = GetConfig( );

        // Close stream if it happens to be open.
        if ( mStream.is_open( ) )
        {
            mStream.close( );
        }

        // We require a filename from the config package to target.
        if ( config.GetFile( ).empty( ) )
        {
            throw std::logic_error(commonThrowStr + "No filename provided.");
        }

        try
        {
            // Attempt to open file stream.  Use append mode, binary write, only allow write (no read).
            mStream.open(config.GetFile( ), std::ios_base::app | std::ios_base::binary | std::ios_base::out);
        }
        catch ( const std::exception& e )
        {
            throw std::runtime_error(commonThrowStr + e.what( ));
        }

        // See if we successfully opened the file and the stream is in a good state.
        if ( !IsStreamGood( ) )
        {
            throw std::runtime_error(
                commonThrowStr +
                "good == " +
                std::to_string(mStream.good( )) +
                ", is_open == " +
                std::to_string(mStream.is_open( )) +
                ", buffer == " +
                StringUtil::UnsignedToString<char>(StringUtil::ConversionType::Hexidecimal, mStream.rdbuf( ))
            );
        }
    }

    // Restore Stream to Good State.
    template <class StreamType>
    bool StreamLogger<StreamType>::RestoreStream( )
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
        }
        catch ( const std::exception& e )
        {
            // Best effort - log recovery failure to stderr.
            // Less frequent reports as more failed attempts occur.
            if ( (failTicker++ % failFreq) == 0 )
            {
                failTicker = 1;
                failFreq++;
                std::wcerr << L"\n\n   " << __FUNCTIONW__ << L" - Failed to restore stream from bad state ";
                std::wcerr << L"(attempt " << ++failTotalCount << "): ";
                std::wcerr << StringUtil::ConvertAndCopy<wchar_t>(e.what( )) << L"\n" << std::endl;
            }

            return false;
        }

        // Successfully recovered - reset fail trackers.
        failTotalCount = 0;
        failTicker = 0;
        failFreq = FAIL_FREQ_START;

        return IsStreamGood( );
    }

    // Flush Buffer Contents To Stream.
    template <class StreamType>
    void StreamLogger<StreamType>::Flush(const VerbosityLevel& lvl)
    {
        if ( !IsStreamGood( ) )
        {
            return;
        }

        // Flush messages to file periodically, or if the message is likely important.
        if ( (mFlushCounter++ % mFlushInterval) == 0 || lvl >= VerbosityLevel::WARN )
        {
            mStream.flush( );
        }
    }

    // Get Color String - VerbosityLevel (FileStream, Narrow)
    template <>
    template <class T>
    const std::basic_string<T>& StreamLogger<FileStream>::GetColorSequence(const VerbosityLevel&) const
    {
        // Color output intended only for console logging.
        static const std::basic_string<T> emptyStr;
        return emptyStr;
    }

    // Get Color String - Color (FileStream, Narrow)
    template <>
    template <class T>
    const std::basic_string<T>& StreamLogger<FileStream>::GetColorSequence(const Color&) const
    {
        // Color output intended only for console logging.
        static const std::basic_string<T> emptyStr;
        return emptyStr;
    }


    // Get Color String - VerbosityLevel (StdOutStream, Narrow)
    template <>
    template <>
    const std::basic_string<char>& StreamLogger<StdOutStream>::GetColorSequence<char>(const VerbosityLevel& lvl) const
    {
        const ConfigPackage& config = GetConfig( );

        if ( !config.OptionEnabled(OptionFlag::LogInColor) )
        {
            static const std::basic_string<char> emptyStr;
            return emptyStr;
        }

        return mColorSequencesA[ColorConverter::ToScalar(config.GetColor(lvl))];
    }

    // Get Color String - VerbosityLevel (StdOutStream, Wide)
    template <>
    template <>
    const std::basic_string<wchar_t>& StreamLogger<StdOutStream>::GetColorSequence<wchar_t>(const VerbosityLevel& lvl) const
    {
        const ConfigPackage& config = GetConfig( );

        if ( !config.OptionEnabled(OptionFlag::LogInColor) )
        {
            static const std::basic_string<wchar_t> emptyStr;
            return emptyStr;
        }

        return mColorSequencesW[ColorConverter::ToScalar(config.GetColor(lvl))];
    }

    // Get Color String - Color (StdOutStream, Narrow)
    template <>
    template <>
    const std::basic_string<char>& StreamLogger<StdOutStream>::GetColorSequence<char>(const Color& clr) const
    {
        const ConfigPackage& config = GetConfig( );

        if ( !config.OptionEnabled(OptionFlag::LogInColor) )
        {
            static const std::basic_string<char> emptyStr;
            return emptyStr;
        }

        return mColorSequencesA[ColorConverter::ToScalar(clr)];
    }

    // Get Color String - Color (StdOutStream, Wide)
    template <>
    template <>
    const std::basic_string<wchar_t>& StreamLogger<StdOutStream>::GetColorSequence<wchar_t>(const Color& clr) const
    {
        const ConfigPackage& config = GetConfig( );

        if ( !config.OptionEnabled(OptionFlag::LogInColor) )
        {
            static const std::basic_string<wchar_t> emptyStr;
            return emptyStr;
        }

        return mColorSequencesW[ColorConverter::ToScalar(clr)];
    }

    // Log Prefixes to Stream.
    template <class StreamType>
    template <class T>
    void StreamLogger<StreamType>::LogPrefixes(const VerbosityLevel& lvl, const std::thread::id& tid)
    {
        std::vector<std::unique_ptr<T[ ]>> prefixes;

        if ( !IsStreamGood( ) )
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

        // Log in color if option is enabled (does nothing for FileLogger specialization).
        if ( GetConfig( ).OptionEnabled(OptionFlag::LogInColor) )
        {
            mStream << GetColorSequence<wchar_t>(lvl).c_str( );
        }

        for ( auto& p : prefixes )
        {
            if ( !IsStreamGood( ) )
            {
                throw std::runtime_error(__FUNCTION__" - Stream in bad state, cannot write message prefix to stream.");
            }

            mStream << p.get( );
        }
    }

    // Log User Message To File.
    template <class StreamType>
    template <class T>
    void StreamLogger<StreamType>::LogMessage(const T* pFormat, va_list pArgs)
    {
        std::unique_ptr<T[ ]> message;

        if ( !IsStreamGood( ) )
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

        mStream << message.get( );

        // If LogInColor is enabled, then return text output to the 
        // original console foreground color, in case other things are writting to stdout.
        if ( GetConfig( ).OptionEnabled(OptionFlag::LogInColor) )
        {
            mStream << GetColorSequence<wchar_t>(Color::DEFAULT);
        }

        mStream << L"\n";
    }

    /// Constructors \\\

    // ConfigPackage Constructor [C] - stdout.
    template <>
    StreamLogger<StdOutStream>::StreamLogger(const ConfigPackage& config) :
        LoggerBase(config),
        mpWideStreamBuffer(std::wcout.rdbuf( )),
        mStream(mpWideStreamBuffer)
    {
        InitializeStream( );
    }

    // ConfigPackage Constructor [C] - file.
    template <>
    StreamLogger<FileStream>::StreamLogger(const ConfigPackage& config) :
        LoggerBase(config),
        mpWideStreamBuffer(nullptr)
    {
        InitializeStream( );
    }

    // ConfigPackage Constructor [M] - stdout.
    template <>
    StreamLogger<StdOutStream>::StreamLogger(ConfigPackage&& config) :
        LoggerBase(std::move(config)),
        mpWideStreamBuffer(std::wcout.rdbuf( )),
        mStream(mpWideStreamBuffer)
    {
        InitializeStream( );
    }

    // ConfigPackage Constructor [M] - file.
    template <>
    StreamLogger<FileStream>::StreamLogger(ConfigPackage&& config) :
        LoggerBase(std::move(config)),
        mpWideStreamBuffer(nullptr)
    {
        InitializeStream( );
    }

    // Move Constructor - stdout.
    template <>
    StreamLogger<StdOutStream>::StreamLogger(StreamLogger&& src) :
        LoggerBase(std::move(src)),
        mpWideStreamBuffer(std::wcout.rdbuf( )),
        mStream(mpWideStreamBuffer)
    {
        InitializeStream( );
    }

    // Move Constructor - file.
    template <>
    StreamLogger<FileStream>::StreamLogger(StreamLogger&& src) :
        LoggerBase(std::move(src)),
        mpWideStreamBuffer(nullptr)
    {
        InitializeStream( );
    }

    /// Destructor \\\

    // Virtual Destructor
    template <class StreamType>
    StreamLogger<StreamType>::~StreamLogger( ) noexcept
    {
        try
        {
            Flush(SLL::VerbosityLevel::FATAL);
        }
        catch ( const std::exception& )
        {
            // Best attempt - nothing more to be done.
        }
    }

    /// Assignment Overload \\\

    // Move Assignment Overload - stdout.
    template <>
    StreamLogger<StdOutStream>& StreamLogger<StdOutStream>::operator=(StreamLogger&& src)
    {
        if ( this == &src )
        {
            throw std::invalid_argument(__FUNCTION__" - Attempted self-assignment.");
        }

        LoggerBase::operator=(std::move(src));

        return *this;
    }

    // Move Assignment Overload - file.
    template <>
    StreamLogger<FileStream>& StreamLogger<FileStream>::operator=(StreamLogger&& src)
    {
        if ( this == &src )
        {
            throw std::invalid_argument(__FUNCTION__" - Attempted self-assignment.");
        }

        LoggerBase::operator=(std::move(src));
        mStream = std::move(src.mStream);

        return *this;
    }

    /// Public Methods \\\

    template <class StreamType>
    template <class T, typename>
    bool StreamLogger<StreamType>::Log(const VerbosityLevel& lvl, const T* pFormat, va_list pArgs)
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
        if ( !IsStreamGood( ) && !RestoreStream( ) )
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
            return IsStreamGood( );
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
            return IsStreamGood( );
        }

        // Flush messages to file periodically, or if the message is likely important.
        Flush(lvl);

        return IsStreamGood( );
    }

    template <class StreamType>
    template <class T, typename>
    bool StreamLogger<StreamType>::Log(const VerbosityLevel& lvl, const T* pFormat, ...)
    {
        va_list pArgs;

        try
        {
            // Get arguments, send to va_list, cleanup, return result.
            va_start(pArgs, pFormat);
            bool ret = Log<T>(lvl, pFormat, pArgs);
            va_end(pArgs);
            return ret;
        }
        catch ( const std::exception& )
        {
            // Cleanup, rethrow exception.
            va_end(pArgs);
            throw;
        }
    }

    /// Explicit Template Instantiations \\\

    // Log Instantiations - Variadic Arguments
    template bool StdOutLogger::Log<char>(const VerbosityLevel& lvl, const char* pFormat, ...);
    template bool StdOutLogger::Log<wchar_t>(const VerbosityLevel& lvl, const wchar_t* pFormat, ...);
    template bool FileLogger::Log<char>(const VerbosityLevel& lvl, const char* pFormat, ...);
    template bool FileLogger::Log<wchar_t>(const VerbosityLevel& lvl, const wchar_t* pFormat, ...);
}