// Class Header
#include <LoggerBase.h>

// Variadic Function Arguments
#include <cstdarg>

// std::put_time
#include <iomanip>

// String Stream
#include <sstream>

namespace SLL
{
    /// Common Private Helper Methods \\\

    // Prefix - Timestamp Format Getter - Narrow
    template <>
    const std::basic_string<char>& LoggerBase::GetTimeFormat( )
    {
        static const std::basic_string<char> timeFormatA = "[%D - %T]  ";
        return timeFormatA;
    }

    // Prefix - Timestamp Format Getter - Wide
    template <>
    const std::basic_string<wchar_t>& LoggerBase::GetTimeFormat( )
    {
        static const std::basic_string<wchar_t> timeFormatW = L"[%D - %T]  ";
        return timeFormatW;
    }

    // Prefix - Thread ID Format Getter - Narrow
    template <>
    const std::basic_string<char>& LoggerBase::GetThreadIDFormat( )
    {
        static const std::basic_string<char> threadIDFormatA = "TID[%X]  ";
        return threadIDFormatA;
    }

    // Prefix - Thread ID Format Getter - Wide
    template <>
    const std::basic_string<wchar_t>& LoggerBase::GetThreadIDFormat( )
    {
        static const std::basic_string<wchar_t> threadIDFormatW = L"TID[%X]  ";
        return threadIDFormatW;
    }

    // Prefix - VerbosityLevel Format Getter - Narrow
    template <>
    const std::basic_string<char>& LoggerBase::GetVerbosityLevelFormat( )
    {
        static const std::basic_string<char> verbosityLevelFormatA = "Type[%s]  ";
        return verbosityLevelFormatA;
    }

    // Prefix - VerbosityLevel Format Getter - Wide
    template <>
    const std::basic_string<wchar_t>& LoggerBase::GetVerbosityLevelFormat( )
    {
        static const std::basic_string<wchar_t> verbosityLevelFormatW = L"Type[%s]  ";
        return verbosityLevelFormatW;
    }

    // Get Local Time String
    template <class T, typename>
    static std::basic_string<T> LoggerBase::GetLocalTime( )
    {
        std::basic_ostringstream<T> oss;
        const std::time_t t = std::time(nullptr);
        std::tm tm;

        if ( localtime_s(&tm, &t) != 0 )
        {
            throw std::runtime_error(__FUNCTION__" - Failed to obtain local-time.");
        }

        oss << std::put_time<T>(&tm, GetTimeFormat<T>( ).c_str( ));
        return oss.str( );
    }

    // Build Time Prefix String
    template<class T, typename>
    std::unique_ptr<T[ ]> LoggerBase::BuildTimePrefix( )
    {
        std::basic_string<T> str(GetLocalTime<T>( ));
        return StringUtil::CopyCString<T>(str.c_str( ), str.size( ) + 1);
    }

    // Extract Thread ID
    unsigned long LoggerBase::ExtractThreadID(const std::thread::id& tid)
    {
        std::basic_ostringstream<char> oss;
        oss << tid;
        const std::basic_string<char>& str = oss.str( );
        if ( str.empty( ) )
        {
            throw std::runtime_error(__FUNCTION__" - Failed to extract thread id.");
        }

        return static_cast<unsigned long>(atol(str.c_str( )));
    }

    // Returns number of characters required to hold formatted string, including null-terminator (narrow).
    template <>
    size_t LoggerBase::GetRequiredBufferLength<char>(const char* pFormat, va_list pArgs)
    {
        if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid format string (nullptr).");
        }

        int i = _vscprintf(pFormat, pArgs);
        return (i < 0) ? 0 : static_cast<size_t>(i) + 1;
    }

    // Returns number of characters required to hold formatted string, including null-terminator (wide).
    template <>
    size_t LoggerBase::GetRequiredBufferLength<wchar_t>(const wchar_t* pFormat, va_list pArgs)
    {
        if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid format string (nullptr).");
        }

        int i = _vscwprintf(pFormat, pArgs);
        return (i < 0) ? 0 : static_cast<size_t>(i) + 1;
    }

    // Fills buffer with formatted string.  Expected to be used in conjunction with GetRequiredBufferLength( ) (narrow).
    template <>
    std::unique_ptr<char[ ]> LoggerBase::StringPrintWrapper<char>(const size_t bufLen, const char* pFormat, va_list pArgs)
    {
        std::unique_ptr<char[ ]> buf;
        int writeLen = 0;

        if ( bufLen == 0 )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid destination buffer length (" + std::to_string(bufLen) + ").");
        }
        else if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid format string (nullptr).");
        }

        buf = std::make_unique<char[ ]>(bufLen);

        writeLen = vsnprintf(buf.get( ), bufLen, pFormat, pArgs);

        if ( writeLen < 0 )
        {
            throw std::runtime_error(
                __FUNCTION__" - vsnprintf failed to print string to buffer.  Returned (" +
                std::to_string(writeLen) +
                ")."
            );
        }
        else if ( writeLen != bufLen - 1 )
        {
            throw std::logic_error(
                __FUNCTION__" - vsnprintf wrote " +
                std::to_string(writeLen) +
                " characters to buffer - expected " +
                std::to_string(bufLen - 1) +
                "."
            );
        }

        return buf;
    }

    // Fills buffer with formatted string.  Expected to be used in conjunction with GetRequiredBufferLength( ) (wide).
    template <>
    std::unique_ptr<wchar_t[ ]> LoggerBase::StringPrintWrapper<wchar_t>(const size_t bufLen, const wchar_t* pFormat, va_list pArgs)
    {
        std::unique_ptr<wchar_t[ ]> buf;
        int writeLen = 0;

        if ( bufLen == 0 )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid destination buffer length (" + std::to_string(bufLen) + ").");
        }
        else if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid format string (nullptr).");
        }

        buf = std::make_unique<wchar_t[ ]>(bufLen);

        writeLen = vswprintf(buf.get( ), bufLen, pFormat, pArgs);

        if ( writeLen < 0 )
        {
            throw std::runtime_error(
                __FUNCTION__" - vswprintf failed to print string to buffer.  Returned (" +
                std::to_string(writeLen) +
                ")."
            );
        }
        else if ( writeLen != bufLen - 1 )
        {
            throw std::logic_error(
                __FUNCTION__" - vswprintf wrote " +
                std::to_string(writeLen) +
                " characters to buffer - expected " +
                std::to_string(bufLen - 1) +
                "."
            );
        }

        return buf;
    }

///
//
//  Suppress static code analysis warnings that are due to a known bug.
//  https://developercommunity.visualstudio.com/content/problem/271477/c-static-analysis-c26495-vbptr-uninitialized.html
//
///
#pragma warning( push )
#pragma warning( disable : 26495 )

    /// Constructors \\\

    // ConfigPackage Constructor [C]
    LoggerBase::LoggerBase(const ConfigPackage& config) :
        mConfig(config)
    { }

    // ConfigPackage Constructor [M]
    LoggerBase::LoggerBase(ConfigPackage&& config) noexcept :
        mConfig(std::move(config))
    { }

    // Move Constructor
    LoggerBase::LoggerBase(LoggerBase&& src) noexcept
    {
        *this = std::move(src);
    }

#pragma warning( pop )

    /// Assignment Overload \\\

    // Move Assignment
    LoggerBase& LoggerBase::operator=(LoggerBase&& src)
    {
        if ( this == &src )
        {
            throw std::invalid_argument(__FUNCTION__" - Attempted self-assignment.");
        }

        mConfig = std::move(src.mConfig);

        return *this;
    }

    /// Common Protected Helper Methods \\\

    // Generates vector of log-ready strings that contains all enabled message-prefix output.
    // e.g., timestamp (mm/dd/yyyy, HH:mm::ss), thread id, verbosity level, etc.
    template <class T, typename>
    std::vector<std::unique_ptr<T[ ]>> LoggerBase::BuildMessagePrefixes(const VerbosityLevel& lvl, const std::thread::id& tid) const
    {
        std::vector<std::unique_ptr<T[ ]>> prefixStrings;

        if ( lvl < VerbosityLevel::BEGIN || lvl >= VerbosityLevel::MAX )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid verbosity level (" + std::to_string(static_cast<VerbosityLevelType>(lvl)) + ").");
        }

        if ( mConfig.OptionsEnabledAny(OptionFlag::PREFIX_MASK) )
        {
            if ( mConfig.OptionEnabled(OptionFlag::LogTimestamp) )
            {
                prefixStrings.push_back(BuildTimePrefix<T>( ));
            }

            if ( mConfig.OptionEnabled(OptionFlag::LogThreadID) )
            {
                prefixStrings.push_back(BuildFormattedMessage<T>(GetThreadIDFormat<T>( ).c_str( ), ExtractThreadID(tid)));
            }

            if ( mConfig.OptionEnabled(OptionFlag::LogVerbosityLevel) )
            {
                prefixStrings.push_back(BuildFormattedMessage<T>(GetVerbosityLevelFormat<T>( ).c_str( ), VerbosityLevelConverter::ToString<T>(lvl).c_str( )));
            }
        }

        return prefixStrings;
    }

    // Builds user's formatted log message (w/ va_list).
    template <class T, typename>
    std::unique_ptr<T[ ]> LoggerBase::BuildFormattedMessage(const T* pFormat, va_list pArgs)
    {
        size_t reqBufLen = 0;

        if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid format string (nullptr)");
        }

        reqBufLen = GetRequiredBufferLength<T>(pFormat, pArgs);

        if ( reqBufLen == 0 )
        {
            throw std::runtime_error(__FUNCTION__" - Failed to get required size for log message buffer.");
        }

        return StringPrintWrapper<T>(reqBufLen, pFormat, pArgs);
    }

    // Build user's formatted log message (w/o va_list).
    template <class T, typename>
    std::unique_ptr<T[ ]> LoggerBase::BuildFormattedMessage(const T* pFormat, ...)
    {
        std::unique_ptr<T[ ]> str;
        va_list pArgs;

        va_start(pArgs, pFormat);

        try
        {
            str = BuildFormattedMessage<T>(pFormat, pArgs);
        }
        catch ( const std::exception& )
        {
            va_end(pArgs);
            throw;
        }

        va_end(pArgs);

        return str;
    }

    /// Common Getter \\\

    const ConfigPackage& LoggerBase::GetConfig( ) const noexcept
    {
        return mConfig;
    }

    /// Explicit Template Instantiation \\\

    // Build Time Prefix String
    template std::unique_ptr<char[ ]> LoggerBase::BuildTimePrefix<char>( );
    template std::unique_ptr<wchar_t[ ]> LoggerBase::BuildTimePrefix<wchar_t>( );

    // Build Message Prefix Strings
    template std::vector<std::unique_ptr<char[ ]>> LoggerBase::BuildMessagePrefixes<char>(const VerbosityLevel&, const std::thread::id&) const;
    template std::vector<std::unique_ptr<wchar_t[ ]>> LoggerBase::BuildMessagePrefixes<wchar_t>(const VerbosityLevel&, const std::thread::id&) const;

    // Build Formatted String
    template std::unique_ptr<char[ ]> LoggerBase::BuildFormattedMessage<char>(const char*, va_list);
    template std::unique_ptr<wchar_t[ ]> LoggerBase::BuildFormattedMessage<wchar_t>(const wchar_t*, va_list);
}