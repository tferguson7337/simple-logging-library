#include <LoggerBaseTests.h>

#include <sstream>

namespace LoggerBaseTests
{
    using SLL::LoggerBase;
    using ::LoggerBaseTests::Tester;

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
            GetRequiredBufferLengthTests::NoFormat<utf8>,
            GetRequiredBufferLengthTests::NoFormat<utf16>,

            // Positive Tests
            GetRequiredBufferLengthTests::NoArgs<utf8>,
            GetRequiredBufferLengthTests::NoArgs<utf16>,

            GetRequiredBufferLengthTests::IntegralArgs<utf8>,
            GetRequiredBufferLengthTests::IntegralArgs<utf16>,

            GetRequiredBufferLengthTests::FloatingArgs<utf8>,
            GetRequiredBufferLengthTests::FloatingArgs<utf16>,

            GetRequiredBufferLengthTests::StringArgs<utf8>,
            GetRequiredBufferLengthTests::StringArgs<utf16>,

            GetRequiredBufferLengthTests::ThreadIDFormat<utf8>,
            GetRequiredBufferLengthTests::ThreadIDFormat<utf16>,

            GetRequiredBufferLengthTests::VerbosityLevelFormat<utf8>,
            GetRequiredBufferLengthTests::VerbosityLevelFormat<utf16>,

            /// String Print Wrapper Tests \\\

            // Negative Tests
            StringPrintWrapperTests::ZeroLength<utf8>,
            StringPrintWrapperTests::ZeroLength<utf16>,

            StringPrintWrapperTests::NoFormat<utf8>,
            StringPrintWrapperTests::NoFormat<utf16>,

            // Positive Tests
            StringPrintWrapperTests::NoArgs<utf8>,
            StringPrintWrapperTests::NoArgs<utf16>,

            StringPrintWrapperTests::IntegralArgs<utf8>,
            StringPrintWrapperTests::IntegralArgs<utf16>,

            StringPrintWrapperTests::FloatingArgs<utf8>,
            StringPrintWrapperTests::FloatingArgs<utf16>,

            StringPrintWrapperTests::StringArgs<utf8>,
            StringPrintWrapperTests::StringArgs<utf16>,

            StringPrintWrapperTests::ThreadIDFormat<utf8>,
            StringPrintWrapperTests::ThreadIDFormat<utf16>,

            StringPrintWrapperTests::VerbosityLevelFormat<utf8>,
            StringPrintWrapperTests::VerbosityLevelFormat<utf16>,

            /// Build Message Prefix(es) Tests \\\

            // Negative Tests
            BuildMessagePrefixesTests::BadVerbosityLevel<utf8>,
            BuildMessagePrefixesTests::BadVerbosityLevel<utf16>,

            // Positive Tests
            BuildMessagePrefixesTests::NoOptions<utf8>,
            BuildMessagePrefixesTests::NoOptions<utf16>,
            
            BuildMessagePrefixesTests::TimeOnly<utf8>,
            BuildMessagePrefixesTests::TimeOnly<utf16>,

            BuildMessagePrefixesTests::ThreadIDOnly<utf8>,
            BuildMessagePrefixesTests::ThreadIDOnly<utf16>,

            BuildMessagePrefixesTests::VerbosityLevelOnly<utf8>,
            BuildMessagePrefixesTests::VerbosityLevelOnly<utf16>,

            BuildMessagePrefixesTests::Time_ThreadID<utf8>,
            BuildMessagePrefixesTests::Time_ThreadID<utf16>,

            BuildMessagePrefixesTests::Time_VerbosityLevel<utf8>,
            BuildMessagePrefixesTests::Time_VerbosityLevel<utf16>,

            BuildMessagePrefixesTests::ThreadID_VerbosityLevel<utf8>,
            BuildMessagePrefixesTests::ThreadID_VerbosityLevel<utf16>,

            BuildMessagePrefixesTests::AllOptions<utf8>,
            BuildMessagePrefixesTests::AllOptions<utf16>,

            /// Build Formatted Message Tests \\\

            // Negative Tests
            BuildFormattedMessageTests::NoFormat<utf8>,
            BuildFormattedMessageTests::NoFormat<utf16>,

            // Positive Tests
            BuildFormattedMessageTests::NoArgs<utf8>,
            BuildFormattedMessageTests::NoArgs<utf16>,

            BuildFormattedMessageTests::IntegralArgs<utf8>,
            BuildFormattedMessageTests::IntegralArgs<utf16>,

            BuildFormattedMessageTests::FloatingArgs<utf8>,
            BuildFormattedMessageTests::FloatingArgs<utf16>,

            BuildFormattedMessageTests::StringArgs<utf8>,
            BuildFormattedMessageTests::StringArgs<utf16>
        };

        return testList;
    }

    /// Format Getter Tests \\\

    UnitTestResult GetTimeFormat( )
    {
        std::basic_string<utf8> narrow;
        std::basic_string<utf16> wide;

        // Get time formats.
        narrow = Tester::GetTimeFormat<utf8>( );
        wide = Tester::GetTimeFormat<utf16>( );

        // Ensure we got the strings, and the lengths match up.
        SUTL_TEST_ASSERT(!narrow.empty( ));
        SUTL_TEST_ASSERT(!wide.empty( ));
        SUTL_TEST_ASSERT(narrow.size( ) == wide.size( ));

        // Ensure that the narrow/wide strings are equivalent.
        std::basic_string<utf16> narrowConvert = StringUtil::ConvertAndCopy<utf16>(narrow);
        std::basic_string<utf8> wideConvert = StringUtil::ConvertAndCopy<utf8>(wide);

        SUTL_TEST_ASSERT(narrow == wideConvert);
        SUTL_TEST_ASSERT(wide == narrowConvert);

        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult GetThreadIDFormat( )
    {
        std::basic_string<utf8> narrow;
        std::basic_string<utf16> wide;

        // Get thread ID formats.
        narrow = Tester::GetThreadIDFormat<utf8>( );
        wide = Tester::GetThreadIDFormat<utf16>( );

        // Ensure we got the strings, and the lengths match up.
        SUTL_TEST_ASSERT(!narrow.empty( ));
        SUTL_TEST_ASSERT(!wide.empty( ));
        SUTL_TEST_ASSERT(narrow.size( ) == wide.size( ));

        // Ensure that the narrow/wide strings are equivalent.
        std::basic_string<utf16> narrowConvert = StringUtil::ConvertAndCopy<utf16>(narrow);
        std::basic_string<utf8> wideConvert = StringUtil::ConvertAndCopy<utf8>(wide);

        SUTL_TEST_ASSERT(narrow == wideConvert);
        SUTL_TEST_ASSERT(wide == narrowConvert);

        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult GetVerbosityLevelFormat( )
    {
        std::basic_string<utf8> narrow;
        std::basic_string<utf16> wide;

        // Get verbosity level formats.
        narrow = Tester::GetVerbosityLevelFormat<utf8>( );
        wide = Tester::GetVerbosityLevelFormat<utf16>( );

        // Ensure we got the strings, and the lengths match up.
        SUTL_TEST_ASSERT(!narrow.empty( ));
        SUTL_TEST_ASSERT(!wide.empty( ));
        SUTL_TEST_ASSERT(narrow.size( ) == wide.size( ));

        // Ensure that the narrow/wide strings are equivalent.
        std::basic_string<utf16> narrowConvert = StringUtil::ConvertAndCopy<utf16>(narrow);
        std::basic_string<utf8> wideConvert = StringUtil::ConvertAndCopy<utf8>(wide);

        SUTL_TEST_ASSERT(narrow == wideConvert);
        SUTL_TEST_ASSERT(wide == narrowConvert);

        SUTL_TEST_SUCCESS( );
    }

    /// Build Time Prefix Tests \\\

    UnitTestResult GetLocalTime( )
    {
        std::basic_string<utf8> narrow;
        std::basic_string<utf16> wide;

        // Get local time strings.
        try
        {
            narrow = Tester::GetLocalTime<utf8>( );
            wide = Tester::GetLocalTime<utf16>( );
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
        std::basic_string<utf16> narrowConvert = StringUtil::ConvertAndCopy<utf16>(narrow);
        std::basic_string<utf8> wideConvert = StringUtil::ConvertAndCopy<utf8>(wide);

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
        std::unique_ptr<utf8[ ]> narrow;
        std::unique_ptr<utf16[ ]> wide;

        size_t nLen = 0;
        size_t wLen = 0;

        // Get time prefix strings
        try
        {
            narrow = Tester::BuildTimePrefix<utf8>( );
            wide = Tester::BuildTimePrefix<utf16>( );
        }
        catch ( const std::exception& e )
        {
            SUTL_TEST_EXCEPTION(e.what( ));
        }

        // Ensure we got the strings.
        SUTL_TEST_ASSERT(narrow);
        SUTL_TEST_ASSERT(wide);

        // Get narrow length.
        for ( const utf8* p = narrow.get( ); *p != '\0'; p++ )
        {
            nLen++;
        }

        // Get wide length.
        for ( const utf16* p = wide.get( ); *p != L'\0'; p++ )
        {
            wLen++;
        }

        // Ensure the lengths match up.
        SUTL_TEST_ASSERT(++nLen == ++wLen);

        std::unique_ptr<utf16[ ]> narrowConvert = StringUtil::ConvertAndCopy<utf16>(narrow.get( ), nLen);
        std::unique_ptr<utf8[ ]> wideConvert = StringUtil::ConvertAndCopy<utf8>(wide.get( ), wLen);

        // Ensure the narrow and wide strings are equivalent.
        SUTL_TEST_ASSERT(memcmp(narrow.get( ), wideConvert.get( ), sizeof(utf8) * nLen) == 0);
        SUTL_TEST_ASSERT(memcmp(wide.get( ), narrowConvert.get( ), sizeof(utf16) * wLen) == 0);

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
            std::unique_ptr<T[ ]> pFormat(StringUtil::ConvertAndCopy<T>("Test string, thread-id specifier %X."));
            size_t len = 0;
            std::basic_ostringstream<utf8> tidStream;

            tidStream << std::this_thread::get_id( );

            unsigned long tid = static_cast<unsigned long>(atol(tidStream.str( ).c_str( )));

            size_t tidLen = 0;
            for ( unsigned long t = tid; t != 0; t >>= 4 )
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

            SUTL_TEST_ASSERT(len == sizeof("Test string, thread-id specifier %X.") + tidLen - 2);

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
        UnitTestResult ZeroLength( )
        {
            bool threw = false;
            std::unique_ptr<T[ ]> buf;

            try
            {
                buf = Tester::StringPrintWrapper<T>(0, StringUtil::ConvertAndCopy<T>("").get( ));
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
            std::unique_ptr<T[ ]> buf;

            try
            {
                buf = Tester::StringPrintWrapper<T>(1, nullptr);
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
            const std::unique_ptr<T[ ]>& expected = f;
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
                buf = Tester::StringPrintWrapper<T>(len, f.get( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(memcmp(expected.get( ), buf.get( ), len * sizeof(T)) == 0);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult IntegralArgs( )
        {
            std::unique_ptr<T[ ]> f(StringUtil::ConvertAndCopy<T>("Test string, integral specifiers %d %zu."));
            const std::unique_ptr<T[ ]> expected(StringUtil::ConvertAndCopy<T>("Test string, integral specifiers -250 1073741824."));
            std::unique_ptr<T[ ]> buf;
            size_t len = 0;

            const int arg1 = -250;
            const size_t arg2 = static_cast<size_t>(1) << 30;

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
                buf = Tester::StringPrintWrapper<T>(len, f.get( ), arg1, arg2);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(memcmp(expected.get( ), buf.get( ), len * sizeof(T)) == 0);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult FloatingArgs( )
        {
            std::unique_ptr<T[ ]> f(StringUtil::ConvertAndCopy<T>("Test string, floating-point specifiers %2.2f %1.5f."));
            const std::unique_ptr<T[ ]> expected(StringUtil::ConvertAndCopy<T>("Test string, floating-point specifiers 1.30 10000000000.00000."));
            std::unique_ptr<T[ ]> buf;
            size_t len = 0;

            double arg1 = 1.296;
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
                buf = Tester::StringPrintWrapper<T>(len, f.get( ), arg1, arg2);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(memcmp(expected.get( ), buf.get( ), len * sizeof(T)) == 0);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult StringArgs( )
        {
            std::unique_ptr<T[ ]> f(StringUtil::ConvertAndCopy<T>("Test string, string specifiers %s %ls."));
            std::unique_ptr<T[ ]> expected(StringUtil::ConvertAndCopy<T>("Test string, string specifiers Test string #1 \"Test string #2\"."));
            std::unique_ptr<T[ ]> buf;
            size_t len = 0;

            std::unique_ptr<T[ ]> arg1(StringUtil::ConvertAndCopy<T>("Test string #1"));
            std::unique_ptr<utf16[ ]> arg2(StringUtil::ConvertAndCopy<utf16>("\"Test string #2\""));

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
                buf = Tester::StringPrintWrapper<T>(len, f.get( ), arg1.get( ), arg2.get( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(memcmp(expected.get( ), buf.get( ), len * sizeof(T)) == 0);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult ThreadIDFormat( )
        {
            auto HexTID = [ ] ( ) -> std::string
            {
                std::basic_ostringstream<utf8> oss;
                oss << std::hex << std::uppercase;
                oss << Tester::ExtractThreadID(std::this_thread::get_id( ));
                return oss.str( );
            };

            std::unique_ptr<T[ ]> f(StringUtil::ConvertAndCopy<T>("Test string, thread-ID specifier %X."));
            std::unique_ptr<T[ ]> expected(StringUtil::ConvertAndCopy<T>(("Test string, thread-ID specifier " + HexTID( ) + ".").c_str( )));

            std::unique_ptr<T[ ]> buf;
            size_t len = 0;
            unsigned long tid = Tester::ExtractThreadID(std::this_thread::get_id( ));

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
                buf = Tester::StringPrintWrapper<T>(len, f.get( ), tid);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(memcmp(expected.get( ), buf.get( ), len * sizeof(T)) == 0);

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
                const std::unique_ptr<T[ ]> expected(
                    StringUtil::ConvertAndCopy<T>(
                    ("Test string, verbosity-level specifier " + SLL::VerbosityLevelConverter::ToString<utf8>(lvl) + ".").c_str( )));

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
                    buf = Tester::StringPrintWrapper<T>(len, f.get( ), verbosityLevelString.c_str( ));
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }

                SUTL_TEST_ASSERT(memcmp(expected.get( ), buf.get( ), len * sizeof(T)) == 0);
            }

            SUTL_TEST_SUCCESS( );
        }
    }

    // Build Message Prefix(es) Tests
    namespace BuildMessagePrefixesTests
    {
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
                str = Tester::BuildFormattedMessage<T>(pFormat.get( ));
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

                str = Tester::BuildFormattedMessage<T>(pFormat.get( ), arg1, arg2);
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

                str = Tester::BuildFormattedMessage<T>(pFormat.get( ), arg1, arg2);
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
                const utf16* arg2 = UTF16_LITERAL("\"Test string #2\"");

                str = Tester::BuildFormattedMessage<T>(pFormat.get( ), arg1.get( ), arg2);
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
