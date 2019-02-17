// Class Declaration
#include <StreamLogger.h>

// For enabling console color logging
// using Window's Virtual Terminal
#include <WindowsConsoleHelper.h>

// STL synchronization
#include <thread>

namespace SLL
{
    /// Non-Member Static Const String-Tuple Vector \\\

    // Color sequences for Windows 10 (Threshold 2 and beyond) console color output.
    // Defined in https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences (Text Formatting)
    const std::vector<SupportedStringTuple> s_vColorSequences
    {
        MAKE_STR_TUPLE("\x1b[30m"),  // BLACK
        MAKE_STR_TUPLE("\x1b[31m"),  // RED
        MAKE_STR_TUPLE("\x1b[32m"),  // GREEN
        MAKE_STR_TUPLE("\x1b[33m"),  // YELLOW
        MAKE_STR_TUPLE("\x1b[34m"),  // BLUE
        MAKE_STR_TUPLE("\x1b[35m"),  // MAGENTA
        MAKE_STR_TUPLE("\x1b[36m"),  // CYAN
        MAKE_STR_TUPLE("\x1b[37m"),  // WHITE

        MAKE_STR_TUPLE("\x1b[90m"),  // BRIGHT_BLACK
        MAKE_STR_TUPLE("\x1b[91m"),  // BRIGHT_RED
        MAKE_STR_TUPLE("\x1b[92m"),  // BRIGHT_GREEN
        MAKE_STR_TUPLE("\x1b[93m"),  // BRIGHT_YELLOW
        MAKE_STR_TUPLE("\x1b[94m"),  // BRIGHT_BLUE
        MAKE_STR_TUPLE("\x1b[95m"),  // BRIGHT_MAGENTA
        MAKE_STR_TUPLE("\x1b[96m"),  // BRIGHT_CYAN
        MAKE_STR_TUPLE("\x1b[97m"),  // BRIGHT_WHITE

        MAKE_STR_TUPLE("\x1b[39m"),  // DEFAULT
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
    void StreamLogger<StdOutStream>::InitializeStream( ) const
    {
        // Ensure the stream has a buffer (stdout).
        if ( !mStream.rdbuf( ) )
        {
            mStream.set_rdbuf(mpUTF16StreamBuffer);

            if ( !mStream.good( ) )
            {
                mStream.clear( );
            }
        }

        if ( GetConfig( ).OptionEnabled(OptionFlag::LogInColor) )
        {
            if ( !WindowsConsoleHelper::Initialize( ) )
            {
                LoggerBase::GetConfig( ).Disable(OptionFlag::LogInColor);
            }
        }

        if ( !IsStreamGood( ) )
        {
            using Base = StringUtil::NumberConversion::Base;

            throw std::runtime_error(
                std::string(__FUNCTION__" - Failed to initialize stream: ") +
                "good == " +
                std::to_string(mStream.good( )) +
                ", buffer == " +
                StringUtil::NumberConversion::ToString<Base::Hexadecimal, utf8>(reinterpret_cast<uintptr_t>(mStream.rdbuf( )))
            );
        }
    }

    // Initialize Stream (file).
    template <>
    void StreamLogger<FileStream>::InitializeStream( ) const
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
            using Base = StringUtil::NumberConversion::Base;

            throw std::runtime_error(
                commonThrowStr +
                "good == " +
                std::to_string(mStream.good( )) +
                ", is_open == " +
                std::to_string(mStream.is_open( )) +
                ", buffer == " +
                StringUtil::NumberConversion::ToString<Base::Hexadecimal, utf8>(reinterpret_cast<uintptr_t>(mStream.rdbuf( )))
            );
        }
    }

    // Restore Stream to Good State.
    template <class StreamType>
    bool StreamLogger<StreamType>::RestoreStream( ) const
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
                std::wcerr << StringUtil::UTFConversion::ToString<utf16>(e.what( )) << L"\n" << std::endl;
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
    void StreamLogger<StreamType>::Flush(_In_ const VerbosityLevel& lvl) const
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
    const std::basic_string<T>& StreamLogger<FileStream>::GetColorSequence(_In_ const VerbosityLevel&) const
    {
        // Color output intended only for console logging.
        static const std::basic_string<T> emptyStr;
        return emptyStr;
    }

    // Get Color String - Color (FileStream, Narrow)
    template <>
    template <class T>
    const std::basic_string<T>& StreamLogger<FileStream>::GetColorSequence(_In_ const Color&) const
    {
        // Color output intended only for console logging.
        static const std::basic_string<T> emptyStr;
        return emptyStr;
    }


    // Get Color String - VerbosityLevel (StdOutStream)
    template <>
    template <class T>
    const std::basic_string<T>& StreamLogger<StdOutStream>::GetColorSequence(_In_ const VerbosityLevel& lvl) const
    {
        const ConfigPackage& config = GetConfig( );

        if ( !config.OptionEnabled(OptionFlag::LogInColor) )
        {
            static const std::basic_string<T> emptyStr;
            return emptyStr;
        }

        return std::get<std::basic_string<T>>(s_vColorSequences[ColorConverter::ToScalar(config.GetColor(lvl))]);
    }

    // Get Color String - Color (StdOutStream)
    template <>
    template <class T>
    const std::basic_string<T>& StreamLogger<StdOutStream>::GetColorSequence(_In_ const Color& clr) const
    {
        const ConfigPackage& config = GetConfig( );

        if ( !config.OptionEnabled(OptionFlag::LogInColor) )
        {
            static const std::basic_string<T> emptyStr;
            return emptyStr;
        }

        return std::get<std::basic_string<T>>(s_vColorSequences[ColorConverter::ToScalar(clr)]);
    }

    template <class StreamType>
    template <class T>
    bool StreamLogger<StreamType>::LogInternal(_In_ const VerbosityLevel& lvl, _In_ const std::thread::id& tid, _In_z_ _Printf_format_string_ const T* pFormat, _In_ va_list pArgs) const
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

        // Check if the stream is still open and in a good state, attempt to recover if not.
        if ( !IsStreamGood( ) && !RestoreStream( ) )
        {
            return false;
        }

        // Log message prefix strings.
        try
        {
            LogPrefixes<T>(lvl, tid);
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

    // Log Prefixes to Stream.
    template <class StreamType>
    template <class T>
    void StreamLogger<StreamType>::LogPrefixes(_In_ const VerbosityLevel& lvl, _In_ const std::thread::id& tid) const
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
            mStream << GetColorSequence<utf16>(lvl).c_str( );
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
    void StreamLogger<StreamType>::LogMessage(_In_z_ _Printf_format_string_ const T* pFormat, _In_ va_list pArgs) const
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
            mStream << GetColorSequence<utf16>(Color::DEFAULT);
        }

        mStream << L"\n";
    }

    template <class StreamType>
    ConfigPackage& StreamLogger<StreamType>::GetConfig( ) noexcept
    {
        return LoggerBase::GetConfig( );
    }

    /// Constructors \\\

    // ConfigPackage Constructor [C] - stdout.
    template <>
    StreamLogger<StdOutStream>::StreamLogger(_In_ const ConfigPackage& config) :
        LoggerBase(config),
        mpUTF16StreamBuffer(reinterpret_cast<std::basic_streambuf<utf16>*>(std::wcout.rdbuf( ))),
        mStream(mpUTF16StreamBuffer)
    {
        InitializeStream( );
    }

    // ConfigPackage Constructor [C] - file.
    template <>
    StreamLogger<FileStream>::StreamLogger(_In_ const ConfigPackage& config) :
        LoggerBase(config),
        mpUTF16StreamBuffer(nullptr)
    {
        InitializeStream( );
    }

    // ConfigPackage Constructor [M] - stdout.
    template <>
    StreamLogger<StdOutStream>::StreamLogger(_In_ ConfigPackage&& config) :
        LoggerBase(std::move(config)),
        mpUTF16StreamBuffer(reinterpret_cast<std::basic_streambuf<utf16>*>(std::wcout.rdbuf( ))),
        mStream(mpUTF16StreamBuffer)
    {
        InitializeStream( );
    }

    // ConfigPackage Constructor [M] - file.
    template <>
    StreamLogger<FileStream>::StreamLogger(_In_ ConfigPackage&& config) :
        LoggerBase(std::move(config)),
        mpUTF16StreamBuffer(nullptr)
    {
        InitializeStream( );
    }

    // Move Constructor - stdout.
    template <>
    StreamLogger<StdOutStream>::StreamLogger(_In_ StreamLogger&& src) :
        LoggerBase(std::move(src)),
        mpUTF16StreamBuffer(reinterpret_cast<std::basic_streambuf<utf16>*>(std::wcout.rdbuf( ))),
        mStream(mpUTF16StreamBuffer)
    {
        InitializeStream( );
    }

    // Move Constructor - file.
    template <>
    StreamLogger<FileStream>::StreamLogger(_In_ StreamLogger&& src) :
        LoggerBase(std::move(src)),
        mpUTF16StreamBuffer(nullptr)
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
    StreamLogger<StdOutStream>& StreamLogger<StdOutStream>::operator=(_In_ StreamLogger&& src)
    {
        if ( this == &src )
        {
            throw std::invalid_argument(__FUNCTION__" - Attempted self-assignment.");
        }

        LoggerBase::operator=(std::move(src));
        src.mStream.set_rdbuf(nullptr);
        mpUTF16StreamBuffer = std::move(src.mpUTF16StreamBuffer);

        return *this;
    }

    // Move Assignment Overload - file.
    template <>
    StreamLogger<FileStream>& StreamLogger<FileStream>::operator=(_In_ StreamLogger&& src)
    {
        if ( this == &src )
        {
            throw std::invalid_argument(__FUNCTION__" - Attempted self-assignment.");
        }

        LoggerBase::operator=(std::move(src));
        mStream = std::move(src.mStream);
        mpUTF16StreamBuffer = std::move(src.mpUTF16StreamBuffer);

        return *this;
    }

    /// Public Getter \\\

    template <class StreamType>
    const ConfigPackage& StreamLogger<StreamType>::GetConfig( ) const noexcept
    {
        return LoggerBase::GetConfig( );
    }

    /// Public Methods \\\

    // Submit log message to stream(s) (variadic arguments, narrow).
    template <class StreamType>
    bool StreamLogger<StreamType>::Log(_In_ const VerbosityLevel& lvl, _In_z_ _Printf_format_string_ const utf8* pFormat, ...) const
    {
        bool ret = false;
        va_list pArgs;
        va_start(pArgs, pFormat);
        try
        {
            ret = LogInternal(lvl, std::this_thread::get_id( ), pFormat, pArgs);
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
    template <class StreamType>
    bool StreamLogger<StreamType>::Log(_In_ const VerbosityLevel& lvl, _In_z_ _Printf_format_string_ const utf16* pFormat, ...) const
    {
        bool ret = false;
        va_list pArgs;
        va_start(pArgs, pFormat);
        try
        {
            ret = LogInternal(lvl, std::this_thread::get_id( ), pFormat, pArgs);
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
    template <class StreamType>
    bool StreamLogger<StreamType>::Log(_In_ const VerbosityLevel& lvl, _In_ const std::thread::id& tid, _In_z_ _Printf_format_string_ const utf8* pFormat, ...) const
    {
        bool ret = false;
        va_list pArgs;
        va_start(pArgs, pFormat);
        try
        {
            ret = LogInternal(lvl, tid, pFormat, pArgs);
        }
        catch ( const std::exception& )
        {
            va_end(pArgs);
            throw;
        }

        va_end(pArgs);
        return ret;
    }

    // Submit log message to stream(s) (variadic arguments, explicit thread ID).
    template <class StreamType>
    bool StreamLogger<StreamType>::Log(_In_ const VerbosityLevel& lvl, _In_ const std::thread::id& tid, _In_z_ _Printf_format_string_ const utf16* pFormat, ...) const
    {
        bool ret = false;
        va_list pArgs;
        va_start(pArgs, pFormat);
        try
        {
            ret = LogInternal(lvl, tid, pFormat, pArgs);
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
    template <class StreamType>
    bool StreamLogger<StreamType>::Log(_In_ const VerbosityLevel& lvl, _In_z_ _Printf_format_string_ const utf8* pFormat, _In_ va_list pArgs) const
    {
        return LogInternal(lvl, std::this_thread::get_id( ), pFormat, pArgs);
    }

    // Submit log message to stream(s) (va_list, wide).
    template <class StreamType>
    bool StreamLogger<StreamType>::Log(_In_ const VerbosityLevel& lvl, _In_z_ _Printf_format_string_  const utf16* pFormat, _In_ va_list pArgs) const
    {
        return LogInternal(lvl, std::this_thread::get_id( ), pFormat , pArgs);
    }

    // Submit log message to stream(s) (va_list, narrow, explicit thread ID).
    template <class StreamType>
    bool StreamLogger<StreamType>::Log(_In_ const VerbosityLevel& lvl, _In_ const std::thread::id& tid, _In_z_ _Printf_format_string_ const utf8* pFormat, _In_ va_list pArgs) const
    {
        return LogInternal(lvl, tid, pFormat, pArgs);
    }

    // Submit log message to stream(s) (va_list, wide, explicit thread ID).
    template <class StreamType>
    bool StreamLogger<StreamType>::Log(_In_ const VerbosityLevel& lvl, _In_ const std::thread::id& tid, _In_z_ _Printf_format_string_ const utf16* pFormat, _In_ va_list pArgs) const
    {
        return LogInternal(lvl, tid, pFormat, pArgs);
    }

    /// Explicit Template Instantiations \\\

    // GetConfig Instantiations
    template ConfigPackage& StdOutLogger::GetConfig( ) noexcept;
    template const ConfigPackage& StdOutLogger::GetConfig( ) const noexcept;
    template ConfigPackage& FileLogger::GetConfig( ) noexcept;
    template const ConfigPackage& FileLogger::GetConfig( ) const noexcept;

    // Log Instantiations - Variadic Arguments
    template bool StdOutLogger::Log(const VerbosityLevel& lvl, const utf8* pFormat, ...) const;
    template bool StdOutLogger::Log(const VerbosityLevel& lvl, const utf16* pFormat, ...) const;
    template bool FileLogger::Log(const VerbosityLevel& lvl, const utf8* pFormat, ...) const;
    template bool FileLogger::Log(const VerbosityLevel& lvl, const utf16* pFormat, ...) const;

    // Log Instantiations - Variadic Arguments, Explicit Thread ID
    template bool StdOutLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const utf8* pFormat, ...) const;
    template bool StdOutLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const utf16* pFormat, ...) const;
    template bool FileLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const utf8* pFormat, ...) const;
    template bool FileLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const utf16* pFormat, ...) const;

    // Log Instantiations - va_list
    template bool StdOutLogger::Log(const VerbosityLevel& lvl, const utf8* pFormat, va_list pArgs) const;
    template bool StdOutLogger::Log(const VerbosityLevel& lvl, const utf16* pFormat, va_list pArgs) const;
    template bool FileLogger::Log(const VerbosityLevel& lvl, const utf8* pFormat, va_list pArgs) const;
    template bool FileLogger::Log(const VerbosityLevel& lvl, const utf16* pFormat, va_list pArgs) const;

    // Log Instantiations - va_list, Explicit Thread ID
    template bool StdOutLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const utf8* pFormat, va_list pArgs) const;
    template bool StdOutLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const utf16* pFormat, va_list pArgs) const;
    template bool FileLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const utf8* pFormat, va_list pArgs) const;
    template bool FileLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const utf16* pFormat, va_list pArgs) const;
}