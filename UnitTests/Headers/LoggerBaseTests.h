#pragma once

#include <LoggerBase.h>

#include <Uncopyable.h>
#include <Unmovable.h>

#include <functional>
#include <list>

#include <UnitTestResult.h>


namespace LoggerBaseTests
{
    std::list<std::function<UnitTestResult(void)>> GetTests( );

    // Format Getter Tests
    UnitTestResult GetTimeFormat( );
    UnitTestResult GetThreadIDFormat( );
    UnitTestResult GetVerbosityLevelFormat( );

    // Build Time Prefix Tests
    UnitTestResult GetLocalTime( );
    UnitTestResult BuildTimePrefix( );

    // Get Required Buffer Length Tests
    namespace GetRequiredBufferLengthTests
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult NoFormat( );

        /// Positive Tests \\\

        template <class T>
        UnitTestResult NoArgs( );

        template <class T>
        UnitTestResult IntegralArgs( );

        template <class T>
        UnitTestResult FloatingArgs( );

        template <class T>
        UnitTestResult StringArgs( );

        template <class T>
        UnitTestResult TimeFormat( );

        template <class T>
        UnitTestResult ThreadIDFormat( );

        template <class T>
        UnitTestResult VerbosityLevelFormat( );
    }

    // String Print Wrapper Tests
    namespace StringPrintWrapperTests
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult ZeroLength( );

        template <class T>
        UnitTestResult NoFormat( );

        /// Positive Tests \\\

        template <class T>
        UnitTestResult NoArgs( );

        template <class T>
        UnitTestResult IntegralArgs( );

        template <class T>
        UnitTestResult FloatingArgs( );

        template <class T>
        UnitTestResult StringArgs( );

        template <class T>
        UnitTestResult ThreadIDFormat( );

        template <class T>
        UnitTestResult VerbosityLevelFormat( );
    }

    // Build Message Prefix(es) Tests
    namespace BuildMessagePrefixesTests
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult BadVerbosityLevel( );

        /// Positive Tests \\\

        template <class T>
        UnitTestResult NoOptions( );

        template <class T>
        UnitTestResult TimeOnly( );

        template <class T>
        UnitTestResult ThreadIDOnly( );

        template <class T>
        UnitTestResult VerbosityLevelOnly( );

        template <class T>
        UnitTestResult Time_ThreadID( );

        template <class T>
        UnitTestResult Time_VerbosityLevel( );

        template <class T>
        UnitTestResult ThreadID_VerbosityLevel( );

        template <class T>
        UnitTestResult AllOptions( );
    }

    // Build Formatted String Tests
    namespace BuildFormattedMessageTests
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult NoFormat( );

        /// Positive Tests \\\

        template <class T>
        UnitTestResult NoArgs( );

        template <class T>
        UnitTestResult IntegralArgs( );

        template <class T>
        UnitTestResult FloatingArgs( );

        template <class T>
        UnitTestResult StringArgs( );
    }
}

// Helper Functions and Macros
namespace LoggerBaseTests
{
    // Helper macros for incrementing enum class variables.
#define INCREMENT_VERBOSITY(v)  v = static_cast<SLL::VerbosityLevel>(static_cast<SLL::VerbosityLevelType>(v) + 1)
#define INCREMENT_OPTIONFLAG(f) f = static_cast<SLL::OptionFlag>(static_cast<SLL::OptionFlagType>(f) + 1)
 
    template <class T>
    bool IsDigit(const T);

    template <>
    inline bool IsDigit<char>(const char c)
    {
        return isdigit(c) != 0;
    }

    template <>
    inline bool IsDigit<wchar_t>(const wchar_t c)
    {
        return iswdigit(c) != 0;
    }

    template <class T>
    bool IsHex(const T);

    template <>
    inline bool IsHex<char>(const char c)
    {
        return isalnum(c) != 0;
    }

    template <>
    inline bool IsHex<wchar_t>(const wchar_t c)
    {
        return iswalnum(c) != 0;
    }

    template <class T>
    bool IsAlpha(const T);

    template <>
    inline bool IsAlpha<char>(const char c)
    {
        return isalpha(c) != 0;
    }

    template <>
    inline bool IsAlpha<wchar_t>(const wchar_t c)
    {
        return iswalpha(c) != 0;
    }

    template <class T>
    bool IsSpace(const T);

    template <>
    inline bool IsSpace<char>(const char c)
    {
        return isspace(c) != 0;
    }

    template <>
    inline bool IsSpace<wchar_t>(const wchar_t c)
    {
        return iswspace(c) != 0;
    }

    template <class T>
    inline bool IsTimePrefix(const std::unique_ptr<T[ ]>& str)
    {
        return (
            str &&
            str[0] == T('[') &&
            IsDigit<T>(str[1]) && IsDigit<T>(str[2]) && str[3] == T('/') &&       // Month
            IsDigit<T>(str[4]) && IsDigit<T>(str[5]) && str[6] == T('/') &&       // Day
            IsDigit<T>(str[7]) && IsDigit<T>(str[8]) &&                           // Year 
            IsSpace<T>(str[9]) && str[10] == T('-') && IsSpace<T>(str[11]) &&     // Separator
            IsDigit<T>(str[12]) && IsDigit<T>(str[13]) && str[14] == T(':') &&    // Hours
            IsDigit<T>(str[15]) && IsDigit<T>(str[16]) && str[17] == T(':') &&    // Minutes
            IsDigit<T>(str[18]) && IsDigit<T>(str[19]) && str[20] == T(']') &&    // Seconds
            IsSpace<T>(str[21]) && IsSpace<T>(str[22]) && str[23] == T('\0')      // Ending spaces
            );

    }

    template <class T>
    inline bool IsThreadIDPrefix(const std::unique_ptr<T[ ]>& str)
    {
        if ( !str )
        {
            return false;
        }

        // TID Prefix (0 - 3)
        if ( !(str[0] == T('T') && str[1] == T('I') && str[2] == T('D') && str[3] == T('[')) )
        {
            return false;
        }

        // Thread ID - Hexademical Digits (4 - 11)
        for ( size_t c = 0; c < 8; c++ )
        {
            if ( !IsHex<T>(str[c + 4]) || str[c + 4] == T('\0') )
            {
                return false;
            }
        }

        // String end (12 - 15).
        return (str[12] == T(']') && IsSpace<T>(str[13]) && IsSpace<T>(str[14]) && str[15] == T('\0'));
    }

    template <class T>
    inline bool IsVerbosityLevelPrefix(const std::unique_ptr<T[ ]>& str)
    {
        bool match = false;

        if ( !str )
        {
            return false;
        }

        // Type prefix (0 - 4)
        if ( !(str[0] == T('T') && str[1] == T('y') && str[2] == T('p') && str[3] == T('e') && str[4] == T('[')) )
        {
            return false;
        }

        // VerbosityLevel string (5 - 9)
        size_t l = 0;
        for ( size_t i = 5; i < 10; i++ )
        {
            if ( !IsAlpha(str[i]) && !IsSpace(str[i]) )
            {
                return false;
            }
            else if ( l == 0 && IsAlpha(str[i]) )
            {
                // Get start of string after space(s).
                l = i;
            }
        }

        // Check for verbosity string match.
        std::basic_string<T> subStr(&str[l], 10 - l);
        for ( SLL::VerbosityLevel lvl = SLL::VerbosityLevel::BEGIN; lvl != SLL::VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
        {
            const std::basic_string<T> verbosityString(SLL::VerbosityLevelConverter::ToString<T>(lvl));
            if ( subStr.size( ) == verbosityString.size( ) && subStr == verbosityString )
            {
                match = true;
                break;
            }
        }

        if ( !match )
        {
            return false;
        }

        // String end (10 - 13).
        return (str[10] == T(']') && IsSpace<T>(str[11]) && IsSpace<T>(str[12]) && str[13] == T('\0'));
    }
}

namespace LoggerBaseTests
{
    ///
    //
    //  Class   - LoggerBaseTester
    //
    //  Purpose - Friend of LoggerBase, exposes non-public methods for testing.
    //
    ///
    class Tester final : public ::SLL::LoggerBase
    {
        Tester(const Tester&) = delete;
        Tester(Tester&&) = delete;
        Tester& operator=(const Tester&) = delete;
        Tester& operator=(Tester&&) = delete;

    public:
        /// Constructors \\\
            
        explicit Tester(const SLL::ConfigPackage& cp) :
            LoggerBase(cp)
        { }

        explicit Tester(SLL::ConfigPackage&& cp) noexcept :
            LoggerBase(std::move(cp))
        { }

        /// Destructors \\\

        virtual ~Tester( ) = default;

        /// LoggerBase Static Wrappers \\\

        // Prefix - Timestamp Format Getter (std::put_time Format)
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static const std::basic_string<T>& GetTimeFormat( )
        {
            return SLL::LoggerBase::GetTimeFormat<T>( );
        }

        // Prefix - Thread ID Format Getter
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static const std::basic_string<T>& GetThreadIDFormat( )
        {
            return SLL::LoggerBase::GetThreadIDFormat<T>( );
        }

        // Prefix - VerbosityLevel Format Getter
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static const std::basic_string<T>& GetVerbosityLevelFormat( )
        {
            return LoggerBase::GetVerbosityLevelFormat<T>( );
        }

        // Extract Thread ID
        static unsigned long ExtractThreadID(const std::thread::id& tid)
        {
            return LoggerBase::ExtractThreadID(tid);
        }

        // Get Local Time String
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static std::basic_string<T> GetLocalTime( )
        {
            return LoggerBase::GetLocalTime<T>( );
        }

        // Build Time Prefix String
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static std::unique_ptr<T[ ]> BuildTimePrefix( )
        {
            return LoggerBase::BuildTimePrefix<T>( );
        }

        // Returns required length of buffer to hold built format-string
        // that would be built using format and arguments.
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static size_t GetRequiredBufferLength(const T* f, ...)
        {
            size_t l = 0;
            va_list a;
            va_start(a, f);
            try
            {
                l = LoggerBase::GetRequiredBufferLength<T>(f, a);
            }
            catch ( const std::invalid_argument& )
            {
                va_end(a);
                throw;
            }
            catch ( const std::exception& )
            {
                va_end(a);
                throw;
            }

            va_end(a);
            return l;
        }

        // Will build format-string with arguments, filling the passed buffer.
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static std::unique_ptr<T[ ]> StringPrintWrapper(const size_t len, const T* f, ...)
        {
            std::unique_ptr<T[ ]> str;
            va_list a;

            va_start(a, f);

            try
            {
                str = LoggerBase::StringPrintWrapper<T>(len, f, a);
            }
            catch ( const std::invalid_argument& )
            {
                va_end(a);
                throw;
            }
            catch ( const std::exception& )
            {
                va_end(a);
                throw;
            }

            va_end(a);

            return str;
        }

        /// LoggerBase Non-Static Wrappers \\\

        // Wrapper for BuildMessagePrefixes.
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        std::vector<std::unique_ptr<T[ ]>> BuildMessagePrefixes(const SLL::VerbosityLevel& lvl, const std::thread::id& tid) const
        {
            return LoggerBase::BuildMessagePrefixes<T>(lvl, tid);
        }

        // Dummy Flush method to satisfy ILogger interface.
        void Flush( )
        {
            __noop;
        }
    };
}
