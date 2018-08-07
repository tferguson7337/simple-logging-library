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
        static const std::basic_string<char> threadIDFormatA = "TID[%u]  ";
        return threadIDFormatA;
    }

    // Prefix - Thread ID Format Getter - Wide
    template <>
    const std::basic_string<wchar_t>& LoggerBase::GetThreadIDFormat( )
    {
        static const std::basic_string<wchar_t> threadIDFormatW = L"TID[%u]  ";
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
    void LoggerBase::StringPrintWrapper<char>(std::unique_ptr<char[ ]>& buf, const size_t bufLen, const char* pFormat, va_list pArgs)
    {
        if ( !buf )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid destination buffer (nullptr).");
        }
        else if ( bufLen == 0 )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid destination buffer length (" + std::to_string(bufLen) + ").");
        }
        else if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid format string (nullptr).");
        }

        vsnprintf(buf.get( ), bufLen, pFormat, pArgs);
    }

    // Fills buffer with formatted string.  Expected to be used in conjunction with GetRequiredBufferLength( ) (wide).
    template <>
    void LoggerBase::StringPrintWrapper<wchar_t>(std::unique_ptr<wchar_t[ ]>& buf, const size_t bufLen, const wchar_t* pFormat, va_list pArgs)
    {
        if ( !buf )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid destination buffer (nullptr).");
        }
        else if ( bufLen == 0 )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid destination buffer length (" + std::to_string(bufLen) + ").");
        }
        else if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid format string (nullptr).");
        }

        vswprintf(buf.get( ), bufLen, pFormat, pArgs);
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

    // Builds user's formatted log message.
    template <class T, typename>
    std::unique_ptr<T[ ]> LoggerBase::BuildFormattedMessage(const T* pFormat, ...)
    {
        std::unique_ptr<T[ ]> str;
        size_t reqBufLen = 0;
        va_list pArgs;

        if ( !pFormat )
        {
            throw std::invalid_argument(__FUNCTION__" - Invalid format string (nullptr)");
        }

        va_start(pArgs, pFormat);

        if ( !pArgs )
        {
            throw std::runtime_error(__FUNCTION__" - Failed to get format string arguments.");
        }

        reqBufLen = GetRequiredBufferLength<T>(pFormat, pArgs);

        if ( reqBufLen == 0 )
        {
            va_end(pArgs);
            throw std::runtime_error(__FUNCTION__" - Failed to get required size for log message buffer.");
        }

        str = std::make_unique<T[ ]>(reqBufLen);
        StringPrintWrapper<T>(str, reqBufLen, pFormat, pArgs);

        va_end(pArgs);

        if ( !str )
        {
            throw std::runtime_error(__FUNCTION__" - Failed to build formatted string.");
        }

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
    template std::unique_ptr<char[ ]> LoggerBase::BuildFormattedMessage<char>(const char*, ...);
    template std::unique_ptr<wchar_t[ ]> LoggerBase::BuildFormattedMessage<wchar_t>(const wchar_t*, ...);
}