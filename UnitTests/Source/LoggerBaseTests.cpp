#include <LoggerBaseTests.h>

#include <sstream>

namespace LoggerBaseTests
{
    using SLL::LoggerBase;
    using SLL::LoggerBaseTests::Tester;

#define INCREMENT_VERBOSITY(v) (v = static_cast<SLL::VerbosityLevel>(static_cast<SLL::VerbosityLevelType>(v) + 1))

    std::list<std::function<UnitTestResult(void)>> GetTests( )
    {
        static const std::list<std::function<UnitTestResult(void)>> testList
        {
            /// Format Getter Tests \\\

            GetTimeFormat,
            GetThreadIDFormat,
            GetVerbosityLevelFormat,

            /// Time Prefix Tests \\\

            BuildTimePrefix,
            GetLocalTime,

            /// Get Required Buffer Length Tests \\\
            
            // Negative Tests
            GetRequiredBufferLengthTests::NoFormat<char>,
            GetRequiredBufferLengthTests::NoFormat<wchar_t>,

            // Positive Tests
            GetRequiredBufferLengthTests::NoArgs<char>,
            GetRequiredBufferLengthTests::NoArgs<wchar_t>,

            GetRequiredBufferLengthTests::IntegralArgs<char>,
            GetRequiredBufferLengthTests::IntegralArgs<wchar_t>,

            GetRequiredBufferLengthTests::FloatingArgs<char>,
            GetRequiredBufferLengthTests::FloatingArgs<wchar_t>,

            GetRequiredBufferLengthTests::StringArgs<char>,
            GetRequiredBufferLengthTests::StringArgs<wchar_t>,

            GetRequiredBufferLengthTests::ThreadIDFormat<char>,
            GetRequiredBufferLengthTests::ThreadIDFormat<wchar_t>,

            GetRequiredBufferLengthTests::VerbosityLevelFormat<char>,
            GetRequiredBufferLengthTests::VerbosityLevelFormat<wchar_t>,

            /// String Print Wrapper Tests \\\

            // Negative Tests
            StringPrintWrapperTests::EmptyBuffer<char>,
            StringPrintWrapperTests::EmptyBuffer<wchar_t>,

            StringPrintWrapperTests::ZeroLength<char>,
            StringPrintWrapperTests::ZeroLength<wchar_t>,

            StringPrintWrapperTests::NoFormat<char>,
            StringPrintWrapperTests::NoFormat<wchar_t>,

            // Positive Tests
            StringPrintWrapperTests::NoArgs<char>,
            StringPrintWrapperTests::NoArgs<wchar_t>,

            StringPrintWrapperTests::IntegralArgs<char>,
            StringPrintWrapperTests::IntegralArgs<wchar_t>,

            StringPrintWrapperTests::FloatingArgs<char>,
            StringPrintWrapperTests::FloatingArgs<wchar_t>,

            StringPrintWrapperTests::StringArgs<char>,
            StringPrintWrapperTests::StringArgs<wchar_t>,

            StringPrintWrapperTests::ThreadIDFormat<char>,
            StringPrintWrapperTests::ThreadIDFormat<wchar_t>,

            StringPrintWrapperTests::VerbosityLevelFormat<char>,
            StringPrintWrapperTests::VerbosityLevelFormat<wchar_t>,

            /// Build Message Prefix(es) Tests \\\

            // Negative Tests
            BuildMessagePrefixesTests::BadVerbosityLevel<char>,
            BuildMessagePrefixesTests::BadVerbosityLevel<wchar_t>,

            // Positive Tests
            BuildMessagePrefixesTests::NoOptions<char>,
            BuildMessagePrefixesTests::NoOptions<wchar_t>,
            
            BuildMessagePrefixesTests::TimeOnly<char>,
            BuildMessagePrefixesTests::TimeOnly<wchar_t>,

            BuildMessagePrefixesTests::ThreadIDOnly<char>,
            BuildMessagePrefixesTests::ThreadIDOnly<wchar_t>,

            BuildMessagePrefixesTests::VerbosityLevelOnly<char>,
            BuildMessagePrefixesTests::VerbosityLevelOnly<wchar_t>,

            BuildMessagePrefixesTests::Time_ThreadID<char>,
            BuildMessagePrefixesTests::Time_ThreadID<wchar_t>,

            BuildMessagePrefixesTests::Time_VerbosityLevel<char>,
            BuildMessagePrefixesTests::Time_VerbosityLevel<wchar_t>,

            BuildMessagePrefixesTests::ThreadID_VerbosityLevel<char>,
            BuildMessagePrefixesTests::ThreadID_VerbosityLevel<wchar_t>,

            BuildMessagePrefixesTests::AllOptions<char>,
            BuildMessagePrefixesTests::AllOptions<wchar_t>,

            /// Build Formatted Message Tests \\\

            // Negative Tests
            BuildFormattedMessageTests::NoFormat<char>,
            BuildFormattedMessageTests::NoFormat<wchar_t>,

            // Positive Tests
            BuildFormattedMessageTests::NoArgs<char>,
            BuildFormattedMessageTests::NoArgs<wchar_t>,

            BuildFormattedMessageTests::IntegralArgs<char>,
            BuildFormattedMessageTests::IntegralArgs<wchar_t>,

            BuildFormattedMessageTests::FloatingArgs<char>,
            BuildFormattedMessageTests::FloatingArgs<wchar_t>,

            BuildFormattedMessageTests::StringArgs<char>,
            BuildFormattedMessageTests::StringArgs<wchar_t>
        };

        return testList;
    }


    /// Format Getter Tests \\\

    UnitTestResult GetTimeFormat( )
    {
        std::basic_string<char> narrow;
        std::basic_string<wchar_t> wide;

        // Get time formats.
        narrow = Tester::GetTimeFormat<char>( );
        wide = Tester::GetTimeFormat<wchar_t>( );

        // Ensure we got the strings, and the lengths match up.
        SUTL_TEST_ASSERT(!narrow.empty( ));
        SUTL_TEST_ASSERT(!wide.empty( ));
        SUTL_TEST_ASSERT(narrow.size( ) == wide.size( ));

        // Ensure that the narrow/wide strings are equivalent.
        std::basic_string<wchar_t> narrowConvert = StringUtil::ConvertAndCopy<wchar_t>(narrow);
        std::basic_string<char> wideConvert = StringUtil::ConvertAndCopy<char>(wide);

        SUTL_TEST_ASSERT(narrow == wideConvert);
        SUTL_TEST_ASSERT(wide == narrowConvert);

        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult GetThreadIDFormat( )
    {
        std::basic_string<char> narrow;
        std::basic_string<wchar_t> wide;

        // Get thread ID formats.
        narrow = Tester::GetThreadIDFormat<char>( );
        wide = Tester::GetThreadIDFormat<wchar_t>( );

        // Ensure we got the strings, and the lengths match up.
        SUTL_TEST_ASSERT(!narrow.empty( ));
        SUTL_TEST_ASSERT(!wide.empty( ));
        SUTL_TEST_ASSERT(narrow.size( ) == wide.size( ));

        // Ensure that the narrow/wide strings are equivalent.
        std::basic_string<wchar_t> narrowConvert = StringUtil::ConvertAndCopy<wchar_t>(narrow);
        std::basic_string<char> wideConvert = StringUtil::ConvertAndCopy<char>(wide);

        SUTL_TEST_ASSERT(narrow == wideConvert);
        SUTL_TEST_ASSERT(wide == narrowConvert);

        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult GetVerbosityLevelFormat( )
    {
        std::basic_string<char> narrow;
        std::basic_string<wchar_t> wide;

        // Get verbosity level formats.
        narrow = Tester::GetVerbosityLevelFormat<char>( );
        wide = Tester::GetVerbosityLevelFormat<wchar_t>( );

        // Ensure we got the strings, and the lengths match up.
        SUTL_TEST_ASSERT(!narrow.empty( ));
        SUTL_TEST_ASSERT(!wide.empty( ));
        SUTL_TEST_ASSERT(narrow.size( ) == wide.size( ));

        // Ensure that the narrow/wide strings are equivalent.
        std::basic_string<wchar_t> narrowConvert = StringUtil::ConvertAndCopy<wchar_t>(narrow);
        std::basic_string<char> wideConvert = StringUtil::ConvertAndCopy<char>(wide);

        SUTL_TEST_ASSERT(narrow == wideConvert);
        SUTL_TEST_ASSERT(wide == narrowConvert);

        SUTL_TEST_SUCCESS( );
    }


    /// Build Time Prefix Tests \\\

    UnitTestResult GetLocalTime( )
    {
        std::basic_string<char> narrow;
        std::basic_string<wchar_t> wide;

        // Get local time strings.
        try
        {
            narrow = Tester::GetLocalTime<char>( );
            wide = Tester::GetLocalTime<wchar_t>( );
        }
        catch ( const std::exception& e )
        {
            SUTL_TEST_EXCEPTION(e.what( ));
        }

        // Ensure we got the strings, and the lengths match up.
        SUTL_TEST_ASSERT(!narrow.empty( ));
        SUTL_TEST_ASSERT(!wide.empty( ));
        SUTL_TEST_ASSERT(narrow.size( ) == wide.size( ));

        // Ensure that the narrow/wide strings are equivalent.
        std::basic_string<wchar_t> narrowConvert = StringUtil::ConvertAndCopy<wchar_t>(narrow);
        std::basic_string<char> wideConvert = StringUtil::ConvertAndCopy<char>(wide);

        ///
        //
        //  It's possible for this step to fail, due to a small amount 
        //  of time passing between getting the narrow and wide local-time strings.
        //
        ///
        SUTL_TEST_ASSERT(narrow == wideConvert);
        SUTL_TEST_ASSERT(wide == narrowConvert);

        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult BuildTimePrefix( )
    {
        std::unique_ptr<char[ ]> narrow;
        std::unique_ptr<wchar_t[ ]> wide;

        size_t nLen = 0;
        size_t wLen = 0;

        // Get time prefix strings
        try
        {
            narrow = Tester::BuildTimePrefix<char>( );
            wide = Tester::BuildTimePrefix<wchar_t>( );
        }
        catch ( const std::exception& e )
        {
            SUTL_TEST_EXCEPTION(e.what( ));
        }

        // Ensure we got the strings.
        SUTL_TEST_ASSERT(narrow);
        SUTL_TEST_ASSERT(wide);

        // Get narrow length.
        for ( const char* p = narrow.get( ); *p != '\0'; p++ )
        {
            nLen++;
        }

        // Get wide length.
        for ( const wchar_t* p = wide.get( ); *p != L'\0'; p++ )
        {
            wLen++;
        }

        // Ensure the lengths match up.
        SUTL_TEST_ASSERT(++nLen == ++wLen);

        std::unique_ptr<wchar_t[ ]> narrowConvert = StringUtil::ConvertAndCopy<wchar_t>(narrow.get( ), nLen);
        std::unique_ptr<char[ ]> wideConvert = StringUtil::ConvertAndCopy<char>(wide.get( ), wLen);

        // Ensure the narrow and wide strings are equivalent.
        SUTL_TEST_ASSERT(memcmp(narrow.get( ), wideConvert.get( ), sizeof(char) * nLen) == 0);
        SUTL_TEST_ASSERT(memcmp(wide.get( ), narrowConvert.get( ), sizeof(wchar_t) * wLen) == 0);

        SUTL_TEST_SUCCESS( );
    }


    // Get Required Buffer Length Tests
    namespace GetRequiredBufferLengthTests
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult NoFormat( )
        {
            bool threw = false;

            try
            {
                Tester::GetRequiredBufferLength<T>(nullptr);
            }
            catch ( const std::invalid_argument& )
            {
                threw = true;
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(threw);

            SUTL_TEST_SUCCESS( );
        }

        /// Positive Tests \\\

        template <class T>
        UnitTestResult NoArgs( )
        {
            std::unique_ptr<T[ ]> pFormat(StringUtil::ConvertAndCopy<T>("Test string, no format specifiers."));
            size_t len = 0;

            try
            {
                len = Tester::GetRequiredBufferLength<T>(pFormat.get( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(len == sizeof("Test string, no format specifiers."));

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult IntegralArgs( )
        {
            std::unique_ptr<T[ ]> pFormat(StringUtil::ConvertAndCopy<T>("Test string, integral specifiers %d %llu."));
            size_t len = 0;

            try
            {
                len = Tester::GetRequiredBufferLength<T>(pFormat.get( ), -5000, 5000LLU);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(len == sizeof("Test string, integral specifiers %d %llu.") + 3);

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult FloatingArgs( )
        {
            std::unique_ptr<T[ ]> pFormat(StringUtil::ConvertAndCopy<T>("Test string, floating-point specifiers %2.2f %1.5f."));
            size_t len = 0;

            try
            {
                len = Tester::GetRequiredBufferLength<T>(pFormat.get( ), 12.5255, -30.1502051);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(len == sizeof("Test string, floating-point specifiers %2.2f %1.5f.") + 4);

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult StringArgs( )
        {
            std::unique_ptr<T[ ]> pFormat(StringUtil::ConvertAndCopy<T>("Test string, string specifier %s."));
            size_t len = 0;

            try
            {
                len = Tester::GetRequiredBufferLength<T>(pFormat.get( ), StringUtil::ConvertAndCopy<T>("[this is the test-string arg]").get( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(len == sizeof("Test string, string specifier %s.") + sizeof("[this is the test-string arg]") - 3);

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult ThreadIDFormat( )
        {
            std::unique_ptr<T[ ]> pFormat(StringUtil::ConvertAndCopy<T>("Test string, thread-id specifier %u."));
            size_t len = 0;
            std::basic_ostringstream<char> tidStream;

            tidStream << std::this_thread::get_id( );

            unsigned long tid = static_cast<unsigned long>(atol(tidStream.str( ).c_str( )));

            size_t tidLen = 0;
            for ( unsigned long t = tid; t != 0; t /= 10 )
            {
                tidLen++;
            }

            try
            {
                len = Tester::GetRequiredBufferLength<T>(pFormat.get( ), tid);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(len == sizeof("Test string, thread-id specifier %u.") + tidLen - 2);

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult VerbosityLevelFormat( )
        {
            std::unique_ptr<T[ ]> pFormat(StringUtil::ConvertAndCopy<T>("Test string, verbosity-level specifier %s."));
            size_t len = 0;

            for ( SLL::VerbosityLevel lvl = SLL::VerbosityLevel::BEGIN; lvl != SLL::VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
            {
                std::basic_string<T> verbosityLevelString = SLL::VerbosityLevelConverter::ToString<T>(lvl);

                try
                {
                    len = Tester::GetRequiredBufferLength<T>(pFormat.get( ), verbosityLevelString.c_str( ));
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }

                SUTL_TEST_ASSERT(len == sizeof("Test string, verbosity-level specifier %s.") + verbosityLevelString.size( ) - 2);
            }

            SUTL_TEST_SUCCESS( );
        }
    }


    // String Print Wrapper Tests
    namespace StringPrintWrapperTests
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult EmptyBuffer( )
        {
            bool threw = false;
            std::unique_ptr<T[ ]> buf;

            try
            {
                Tester::StringPrintWrapper<T>(buf, 1, StringUtil::ConvertAndCopy<T>("").get( ));
            }
            catch ( const std::invalid_argument& )
            {
                threw = true;
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(threw);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult ZeroLength( )
        {
            bool threw = false;
            std::unique_ptr<T[ ]> buf(std::make_unique<T[ ]>(1));

            try
            {
                Tester::StringPrintWrapper<T>(buf, 0, StringUtil::ConvertAndCopy<T>("").get( ));
            }
            catch ( const std::invalid_argument& )
            {
                threw = true;
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(threw);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult NoFormat( )
        {
            bool threw = false;
            size_t len = 1;
            std::unique_ptr<T[ ]> buf(std::make_unique<T[ ]>(len));

            try
            {
                Tester::StringPrintWrapper<T>(buf, len, nullptr);
            }
            catch ( const std::invalid_argument& )
            {
                threw = true;
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(threw);

            SUTL_TEST_SUCCESS( );
        }


        /// Positive Tests \\\

        template <class T>
        UnitTestResult NoArgs( )
        {
            std::unique_ptr<T[ ]> f(StringUtil::ConvertAndCopy<T>("Test string, no format specifiers."));
            std::unique_ptr<T[ ]> buf;
            size_t len = 0;

            try
            {
                len = Tester::GetRequiredBufferLength<T>(f.get( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                buf = std::make_unique<T[ ]>(len);
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                Tester::StringPrintWrapper(buf, len, f.get( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult IntegralArgs( )
        {
            std::unique_ptr<T[ ]> f(StringUtil::ConvertAndCopy<T>("Test string, integral specifiers %d %zu."));
            std::unique_ptr<T[ ]> buf;
            size_t len = 0;

            int arg1 = -250;
            size_t arg2 = static_cast<size_t>(1) << 30;

            try
            {
                len = Tester::GetRequiredBufferLength<T>(f.get( ), arg1, arg2);
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                buf = std::make_unique<T[ ]>(len);
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                Tester::StringPrintWrapper(buf, len, f.get( ), arg1, arg2);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult FloatingArgs( )
        {
            std::unique_ptr<T[ ]> f(StringUtil::ConvertAndCopy<T>("Test string, floating-point specifiers %2.2f %1.5."));
            std::unique_ptr<T[ ]> buf;
            size_t len = 0;

            double arg1 = 1.295;
            long double arg2 = std::powl(10, 10);

            try
            {
                len = Tester::GetRequiredBufferLength<T>(f.get( ), arg1, arg2);
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                buf = std::make_unique<T[ ]>(len);
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                Tester::StringPrintWrapper(buf, len, f.get( ), arg1, arg2);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult StringArgs( )
        {
            std::unique_ptr<T[ ]> f(StringUtil::ConvertAndCopy<T>("Test string, string specifiers %s %s."));
            std::unique_ptr<T[ ]> buf;
            size_t len = 0;

            std::unique_ptr<T[ ]> arg1(StringUtil::ConvertAndCopy<T>("Test string #1"));
            std::unique_ptr<wchar_t[ ]> arg2(StringUtil::ConvertAndCopy<wchar_t>("Test string #2"));

            try
            {
                len = Tester::GetRequiredBufferLength<T>(f.get( ), arg1.get( ), arg2.get( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                buf = std::make_unique<T[ ]>(len);
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                Tester::StringPrintWrapper(buf, len, f.get( ), arg1.get( ), arg2.get( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult ThreadIDFormat( )
        {
            std::unique_ptr<T[ ]> f(StringUtil::ConvertAndCopy<T>("Test string, thread-ID specifier %lu."));
            std::unique_ptr<T[ ]> buf;
            size_t len = 0;
            unsigned long tid = 0;

            std::basic_stringstream<char> tidStream;
            tidStream << std::this_thread::get_id( );
            tid = static_cast<unsigned long>(std::atol(tidStream.str( ).c_str( )));

            try
            {
                len = Tester::GetRequiredBufferLength<T>(f.get( ), tid);
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                buf = std::make_unique<T[ ]>(len);
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                Tester::StringPrintWrapper(buf, len, f.get( ), tid);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult VerbosityLevelFormat( )
        {
            std::unique_ptr<T[ ]> f(StringUtil::ConvertAndCopy<T>("Test string, verbosity-level specifier %s."));
            std::unique_ptr<T[ ]> buf;
            size_t len = 0;

            for ( SLL::VerbosityLevel lvl = SLL::VerbosityLevel::BEGIN; lvl != SLL::VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
            {
                const std::basic_string<T> verbosityLevelString(SLL::VerbosityLevelConverter::ToString<T>(lvl));

                try
                {
                    len = Tester::GetRequiredBufferLength<T>(f.get( ), verbosityLevelString.c_str( ));
                }
                catch ( const std::exception& e )
                {
                    SUTL_SETUP_EXCEPTION(e.what( ));
                }

                try
                {
                    buf = std::make_unique<T[ ]>(len);
                }
                catch ( const std::exception& e )
                {
                    SUTL_SETUP_EXCEPTION(e.what( ));
                }

                try
                {
                    Tester::StringPrintWrapper(buf, len, f.get( ), verbosityLevelString.c_str( ));
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }
            }

            SUTL_TEST_SUCCESS( );
        }

    }


    // Build Message Prefix(es) Tests
    namespace BuildMessagePrefixesTests
    {
        template <class T>
        bool IsDigit(const T);

        template <>
        bool IsDigit<char>(const char c)
        {
            return isdigit(c) != 0;
        }

        template <>
        bool IsDigit<wchar_t>(const wchar_t c)
        {
            return iswdigit(c) != 0;
        }

        template <class T>
        bool IsSpace(const T);

        template <>
        bool IsSpace<char>(const char c)
        {
            return isspace(c) != 0;
        }

        template <>
        bool IsSpace<wchar_t>(const wchar_t c)
        {
            return iswspace(c) != 0;
        }

        template <class T>
        bool IsTimePrefix(const std::unique_ptr<T[ ]>& str)
        {
            if ( !str )
            {
                return false;
            }

            // Expected Time Format:
            // "[MM/DD/YYYY - HH:MM:SS]  "
            size_t len = 0;
            while ( str[len] )
            {
                len++;
            }

            if ( ++len != 24 )
            {
                return false;
            }

            return (
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
        bool IsThreadIDPrefix(const std::unique_ptr<T[ ]>& str)
        {
            if ( !str )
            {
                return false;
            }

            // Expected Thread-ID Format:
            // "TID[<unsigned long>]  "
            size_t len = 0;
            while ( str[len] )
            {
                len++;
            }

            if ( ++len < Tester::GetThreadIDFormat<T>( ).size( ) )
            {
                return false;
            }

            // TID Prefix
            if ( !(str[0] == T('T') && str[1] == T('I') && str[2] == T('D') && str[3] == T('[')) )
            {
                return false;
            }

            // Thread ID
            size_t i = 4;
            while ( str[i] != T(']') )
            {
                if ( !IsDigit<T>(str[i]) || str[i] == T('\0') )
                {
                    return false;
                }

                i++;
            }

            // String end.
            return (str[i] == T(']') && IsSpace<T>(str[i + 1]) && IsSpace<T>(str[i + 2]) && str[i + 3] == T('\0'));
        }

        template <class T>
        bool IsVerbosityLevelPrefix(const std::unique_ptr<T[ ]>& str)
        {
            bool match = false;
            size_t i = 0;
            size_t end = 0;

            if ( !str )
            {
                return false;
            }

            // Expected VerbosityLevel Format:
            // "Type[<verbosity level string>]  "
            size_t len = 0;
            while ( str[len] )
            {
                len++;
            }

            if ( ++len < Tester::GetVerbosityLevelFormat<T>( ).size( ) )
            {
                return false;
            }

            // Type prefix
            if ( !(str[0] == T('T') && str[1] == T('y') && str[2] == T('p') && str[3] == T('e') && str[4] == T('[')) )
            {
                return false;
            }

            // Find verbosity string boundaries.
            i = end = 5;
            while ( str[end] != T(']') )
            {
                if ( str[end++] == T('\0') )
                {
                    return false;
                }
            }

            // Check for verbosity string match.
            std::basic_string<T> subStr(&str[i], end - i);
            for ( SLL::VerbosityLevel lvl = SLL::VerbosityLevel::BEGIN; lvl != SLL::VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
            {
                const std::basic_string<T> verbosityString(SLL::VerbosityLevelConverter::ToString<T>(lvl));
                if ( subStr.size( ) == verbosityString.size( ) && subStr == verbosityString )
                {
                    match = true;
                    i += (end - i);
                    break;
                }
            }

            if ( !match )
            {
                return false;
            }

            // String end.
            return (str[i] == T(']') && IsSpace<T>(str[i + 1]) && IsSpace<T>(str[i + 2]) && str[i + 3] == T('\0'));
        }

        /// Negative Tests \\\

        template <class T>
        UnitTestResult BadVerbosityLevel( )
        {
            bool threw = false;
            SLL::ConfigPackage config;
            Tester t(std::move(config));

            try
            {
                t.BuildMessagePrefixes<T>(SLL::VerbosityLevel::MAX, std::this_thread::get_id( ));
            }
            catch ( const std::invalid_argument& )
            {
                threw = true;
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(threw);

            SUTL_TEST_SUCCESS( );
        }


        /// Positive Tests \\\

        template <class T>
        UnitTestResult NoOptions( )
        {
            std::vector<std::unique_ptr<T[ ]>> prefixes;
            SLL::ConfigPackage config;

            SUTL_SETUP_ASSERT(config.GetOptionFlags( ) == SLL::OptionFlag::NONE);

            Tester t(std::move(config));

            for ( SLL::VerbosityLevel lvl = SLL::VerbosityLevel::BEGIN; lvl != SLL::VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
            {
                try
                {
                    prefixes = t.BuildMessagePrefixes<T>(lvl, std::this_thread::get_id( ));
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }

                SUTL_TEST_ASSERT(prefixes.empty( ));
            }

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult TimeOnly( )
        {
            std::vector<std::unique_ptr<T[ ]>> prefixes;
            SLL::ConfigPackage config;

            config.Enable(SLL::OptionFlag::LogTimestamp);

            SUTL_SETUP_ASSERT(config.OptionsEnabledAll(SLL::OptionFlag::LogTimestamp));

            Tester t(std::move(config));

            for ( SLL::VerbosityLevel lvl = SLL::VerbosityLevel::BEGIN; lvl != SLL::VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
            {
                try
                {
                    prefixes = t.BuildMessagePrefixes<T>(lvl, std::this_thread::get_id( ));
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }

                SUTL_TEST_ASSERT(!prefixes.empty( ));
                SUTL_TEST_ASSERT(prefixes.size( ) == 1);

                SUTL_TEST_ASSERT(IsTimePrefix<T>(prefixes[0]));
            }

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult ThreadIDOnly( )
        {
            std::vector<std::unique_ptr<T[ ]>> prefixes;
            SLL::ConfigPackage config;

            config.Enable(SLL::OptionFlag::LogThreadID);

            SUTL_SETUP_ASSERT(config.OptionsEnabledAll(SLL::OptionFlag::LogThreadID));

            Tester t(std::move(config));

            for ( SLL::VerbosityLevel lvl = SLL::VerbosityLevel::BEGIN; lvl != SLL::VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
            {
                try
                {
                    prefixes = t.BuildMessagePrefixes<T>(lvl, std::this_thread::get_id( ));
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }

                SUTL_TEST_ASSERT(!prefixes.empty( ));
                SUTL_TEST_ASSERT(prefixes.size( ) == 1);

                SUTL_TEST_ASSERT(IsThreadIDPrefix<T>(prefixes[0]));
            }

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult VerbosityLevelOnly( )
        {
            std::vector<std::unique_ptr<T[ ]>> prefixes;
            SLL::ConfigPackage config;

            config.Enable(SLL::OptionFlag::LogVerbosityLevel);

            SUTL_SETUP_ASSERT(config.OptionsEnabledAll(SLL::OptionFlag::LogVerbosityLevel));

            Tester t(std::move(config));

            for ( SLL::VerbosityLevel lvl = SLL::VerbosityLevel::BEGIN; lvl != SLL::VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
            {
                try
                {
                    prefixes = t.BuildMessagePrefixes<T>(lvl, std::this_thread::get_id( ));
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }

                SUTL_TEST_ASSERT(!prefixes.empty( ));
                SUTL_TEST_ASSERT(prefixes.size( ) == 1);

                SUTL_TEST_ASSERT(IsVerbosityLevelPrefix<T>(prefixes[0]));
            }

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult Time_ThreadID( )
        {
            std::vector<std::unique_ptr<T[ ]>> prefixes;
            SLL::ConfigPackage config;

            config.Enable(SLL::OptionFlag::LogTimestamp | SLL::OptionFlag::LogThreadID);

            SUTL_SETUP_ASSERT(config.OptionsEnabledAll(SLL::OptionFlag::LogTimestamp | SLL::OptionFlag::LogThreadID));

            Tester t(std::move(config));

            for ( SLL::VerbosityLevel lvl = SLL::VerbosityLevel::BEGIN; lvl != SLL::VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
            {
                try
                {
                    prefixes = t.BuildMessagePrefixes<T>(lvl, std::this_thread::get_id( ));
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }

                SUTL_TEST_ASSERT(!prefixes.empty( ));
                SUTL_TEST_ASSERT(prefixes.size( ) == 2);

                SUTL_TEST_ASSERT(IsTimePrefix<T>(prefixes[0]));
                SUTL_TEST_ASSERT(IsThreadIDPrefix<T>(prefixes[1]));
            }

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult Time_VerbosityLevel( )
        {
            std::vector<std::unique_ptr<T[ ]>> prefixes;
            SLL::ConfigPackage config;

            config.Enable(SLL::OptionFlag::LogTimestamp | SLL::OptionFlag::LogVerbosityLevel);

            SUTL_SETUP_ASSERT(config.OptionsEnabledAll(SLL::OptionFlag::LogTimestamp | SLL::OptionFlag::LogVerbosityLevel));

            Tester t(std::move(config));

            for ( SLL::VerbosityLevel lvl = SLL::VerbosityLevel::BEGIN; lvl != SLL::VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
            {
                try
                {
                    prefixes = t.BuildMessagePrefixes<T>(lvl, std::this_thread::get_id( ));
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }

                SUTL_TEST_ASSERT(!prefixes.empty( ));
                SUTL_TEST_ASSERT(prefixes.size( ) == 2);

                SUTL_TEST_ASSERT(IsTimePrefix<T>(prefixes[0]));
                SUTL_TEST_ASSERT(IsVerbosityLevelPrefix<T>(prefixes[1]));
            }

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult ThreadID_VerbosityLevel( )
        {
            std::vector<std::unique_ptr<T[ ]>> prefixes;
            SLL::ConfigPackage config;

            config.Enable(SLL::OptionFlag::LogThreadID | SLL::OptionFlag::LogVerbosityLevel);

            SUTL_SETUP_ASSERT(config.OptionsEnabledAll(SLL::OptionFlag::LogThreadID | SLL::OptionFlag::LogVerbosityLevel));

            Tester t(std::move(config));

            for ( SLL::VerbosityLevel lvl = SLL::VerbosityLevel::BEGIN; lvl != SLL::VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
            {
                try
                {
                    prefixes = t.BuildMessagePrefixes<T>(lvl, std::this_thread::get_id( ));
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }

                SUTL_TEST_ASSERT(!prefixes.empty( ));
                SUTL_TEST_ASSERT(prefixes.size( ) == 2);

                SUTL_TEST_ASSERT(IsThreadIDPrefix<T>(prefixes[0]));
                SUTL_TEST_ASSERT(IsVerbosityLevelPrefix<T>(prefixes[1]));
            }

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult AllOptions( )
        {
            std::vector<std::unique_ptr<T[ ]>> prefixes;
            SLL::ConfigPackage config;

            config.Enable(SLL::OptionFlag::PREFIX_MASK);

            SUTL_SETUP_ASSERT(config.OptionsEnabledAll(SLL::OptionFlag::PREFIX_MASK));

            Tester t(std::move(config));

            for ( SLL::VerbosityLevel lvl = SLL::VerbosityLevel::BEGIN; lvl != SLL::VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
            {
                try
                {
                    prefixes = t.BuildMessagePrefixes<T>(lvl, std::this_thread::get_id( ));
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }

                SUTL_TEST_ASSERT(!prefixes.empty( ));
                SUTL_TEST_ASSERT(prefixes.size( ) == 3);

                SUTL_TEST_ASSERT(IsTimePrefix<T>(prefixes[0]));
                SUTL_TEST_ASSERT(IsThreadIDPrefix<T>(prefixes[1]));
                SUTL_TEST_ASSERT(IsVerbosityLevelPrefix<T>(prefixes[2]));
            }

            SUTL_TEST_SUCCESS( );
        }

    }


    // Build Formatted Message Tests
    namespace BuildFormattedMessageTests
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult NoFormat( )
        {
            bool threw = false;

            try
            {
                Tester::BuildFormattedMessage<T>(nullptr);
            }
            catch ( const std::invalid_argument& )
            {
                threw = true;
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(threw);

            SUTL_TEST_SUCCESS( );
        }


        /// Positive Tests \\\

        template <class T>
        UnitTestResult NoArgs( )
        {
            const std::unique_ptr<T[ ]> pFormat(StringUtil::ConvertAndCopy<T>("Test string, no format specifiers."));
            const std::unique_ptr<T[ ]> expectedStr(StringUtil::ConvertAndCopy<T>("Test string, no format specifiers."));
            std::unique_ptr<T[ ]> str;

            try
            {
                str = LoggerBase::BuildFormattedMessage<T>(pFormat.get( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(memcmp(expectedStr.get( ), str.get( ), sizeof(T) * sizeof("Test string, no format specifiers.")) == 0);

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult IntegralArgs( )
        {
            const std::unique_ptr<T[ ]> pFormat(StringUtil::ConvertAndCopy<T>("Test string, integral specifiers %d %zu"));
            const std::unique_ptr<T[ ]> expectedStr(StringUtil::ConvertAndCopy<T>("Test string, integral specifiers -5000 5000"));
            std::unique_ptr<T[ ]> str;

            try
            {
                int arg1 = -5000;
                size_t arg2 = 5000;

                str = LoggerBase::BuildFormattedMessage<T>(pFormat.get( ), arg1, arg2);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(memcmp(expectedStr.get( ), str.get( ), sizeof(T) * sizeof("Test string, integral specifiers -5000 5000")) == 0);

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult FloatingArgs( )
        {
            const std::unique_ptr<T[ ]> pFormat(StringUtil::ConvertAndCopy<T>("Test string, floating-point specifiers %2.2f %1.5f."));
            const std::unique_ptr<T[ ]> expectedStr(StringUtil::ConvertAndCopy<T>("Test string, floating-point specifiers 12.53 -30.15021."));
            std::unique_ptr<T[ ]> str;

            try
            {
                float arg1 = 12.5255f;
                double arg2 = -30.1502051;

                str = LoggerBase::BuildFormattedMessage<T>(pFormat.get( ), arg1, arg2);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(memcmp(expectedStr.get( ), str.get( ), sizeof(T) * sizeof("Test string, floating-point specifiers 12.53 -30.15021.")) == 0);

            SUTL_TEST_SUCCESS( );
        }


        template <class T>
        UnitTestResult StringArgs( )
        {
            const std::unique_ptr<T[ ]> pFormat(StringUtil::ConvertAndCopy<T>("Test string, string specifiers %s %ls."));
            const std::unique_ptr<T[ ]> expectedStr(StringUtil::ConvertAndCopy<T>("Test string, string specifiers Test string #1 \"Test string #2\"."));
            std::unique_ptr<T[ ]> str;

            try
            {
                const std::unique_ptr<T[ ]> arg1 = StringUtil::ConvertAndCopy<T>("Test string #1");
                const wchar_t* arg2 = L"\"Test string #2\"";

                str = LoggerBase::BuildFormattedMessage<T>(pFormat.get( ), arg1.get( ), arg2);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(memcmp(expectedStr.get( ), str.get( ), sizeof(T) * sizeof("Test string, string specifiers Test string #1 \"Test string #2\".")) == 0);

            SUTL_TEST_SUCCESS( );
        }

    }
}
