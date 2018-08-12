// Target Test Declarations
#include <FileLoggerTests.h>

namespace FileLoggerTests
{
    using SLL::ConfigPackage;
    using SLL::FileLogger;
    using SLL::OptionFlag;
    using SLL::VerbosityLevel;

    std::list<std::function<UnitTestResult(void)>> GetTests( )
    {
        static const std::list<std::function<UnitTestResult(void)>> testList
        {
            /// InitializeFileStream Tests \\\

            // Negative Tests
            InitializeFileStream::NoFile,
            InitializeFileStream::BadPath,

            // Positive Test
            InitializeFileStream::GoodPath,

            /// RestoreFileStream Tests \\\

            // Negative Tests
            RestoreFileStream::NoFile,
            RestoreFileStream::BadPath,

            // Positive Test
            RestoreFileStream::GoodPath,

            /// Flush Tests \\\

            // Negative Tests
            Flush::NoFile,
            Flush::BadStream,

            // Positive Tests
            Flush::NoContent,
            Flush::GoodStream,

            /// LogPrefixes Tests \\\

            // Negative Tests
            LogPrefixes::NoFile<char>,
            LogPrefixes::NoFile<wchar_t>,

            LogPrefixes::BadStream<char>,
            LogPrefixes::BadStream<wchar_t>,

            LogPrefixes::BadVerbosityLevel<char>,
            LogPrefixes::BadVerbosityLevel<wchar_t>,

            // Positive Tests
            LogPrefixes::GoodStream<char>,
            LogPrefixes::GoodStream<wchar_t>,

            /// LogMessage Tests \\\

            // Negative Tests
            LogMessage::NoFile<char>,
            LogMessage::NoFile<wchar_t>,

            LogMessage::BadStream<char>,
            LogMessage::BadStream<wchar_t>,

            LogMessage::BadFormat<char>,
            LogMessage::BadFormat<wchar_t>,

            // Positive Tests
            LogMessage::ZeroLengthFormat<char>,
            LogMessage::ZeroLengthFormat<wchar_t>,

            LogMessage::PlainFormat<char>,
            LogMessage::PlainFormat<wchar_t>,

            LogMessage::IntegralFormat<char>,
            LogMessage::IntegralFormat<wchar_t>,

            LogMessage::FloatingFormat<char>,
            LogMessage::FloatingFormat<wchar_t>,

            LogMessage::StringFormat<char>,
            LogMessage::StringFormat<wchar_t>,

            /// Log Tests \\\

            // Negative Tests
            Log::BadVerbosityLevel<char>,
            Log::BadVerbosityLevel<wchar_t>,

            Log::BadFormat<char>,
            Log::BadFormat<wchar_t>,

            // Positive Tests
            Log::ZeroLengthFormat<char>,
            Log::ZeroLengthFormat<wchar_t>,

            Log::PlainFormat<char>,
            Log::PlainFormat<wchar_t>,

            Log::IntegralFormat<char>,
            Log::IntegralFormat<wchar_t>,

            Log::FloatingFormat<char>,
            Log::FloatingFormat<wchar_t>,

            Log::StringFormat<char>,
            Log::StringFormat<wchar_t>,

            Log::RecoverFromNotOpen<char>,
            Log::RecoverFromNotOpen<wchar_t>,

            Log::RecoverFromBadState<char>,
            Log::RecoverFromBadState<wchar_t>,

            Log::NoLogBelowThreshold<char>,
            Log::NoLogBelowThreshold<wchar_t>,

            Log::FlushImportantMessage<char>,
            Log::FlushImportantMessage<wchar_t>,

            Log::AppendFileWrite<char>,
            Log::AppendFileWrite<wchar_t>,
        };

        return testList;
    }

    /// Helper Macros \\\

// Perform setup common to most unit tests.
//  - Build new file logger, target (hopefully) good log test-file.
//  - Setup-asserts to ensure target file is open and file stream is in a good state.
#define FILE_LOGGER_TEST_COMMON_SETUP(t)                                                \
    try                                                                                 \
    {                                                                                   \
        t.SetFileLogger(BuildConfig(GetGoodFileString( ), SLL::VerbosityLevel::INFO));  \
    }                                                                                   \
    catch ( const std::exception& e )                                                   \
    {                                                                                   \
        SUTL_SETUP_EXCEPTION(e.what( ));                                                \
    }                                                                                   \
                                                                                        \
    SUTL_SETUP_ASSERT(!t.GetConfig( ).GetFile( ).empty( ));                             \
    SUTL_SETUP_ASSERT(t.GetFileStream( ).good( ));                                      \
    SUTL_SETUP_ASSERT(t.GetFileStream( ).is_open( ));


// Perform cleanup common to most unit tests.
//  - Close file stream.
//  - Attempt to delete the test-log file.
#define FILE_LOGGER_TEST_COMMON_CLEANUP(t)      \
    try                                         \
    {                                           \
        t.GetFileStream( ).close( );            \
        SUTL_CLEANUP_ASSERT(DeleteTestFile( )); \
    }                                           \
    catch ( const std::exception& e )           \
    {                                           \
        SUTL_CLEANUP_EXCEPTION(e.what( ));      \
    }


    /// Unit Tests \\\

    namespace InitializeFileStream
    {
        /// Negative Tests \\\

        UnitTestResult NoFile( )
        {
            bool threw = false;
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                t.GetFileStream( ).close( );
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            SUTL_SETUP_ASSERT(!t.GetFileStream( ).is_open( ));

            t.SetTargetFile(L"");

            try
            {
                t.InitializeFileStream( );
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_TEST_ASSERT(threw);
            SUTL_TEST_ASSERT(!t.GetFileStream( ).is_open( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult BadPath( )
        {
            bool threw = false;
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                t.GetFileStream( ).close( );
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            SUTL_SETUP_ASSERT(!t.GetFileStream( ).is_open( ));

            t.SetTargetFile(GetBadFileString( ));

            try
            {
                t.InitializeFileStream( );
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_TEST_ASSERT(threw);
            SUTL_TEST_ASSERT(!t.GetFileStream( ).is_open( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        /// Positive Tests \\\

        UnitTestResult GoodPath( )
        {
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                t.GetFileStream( ).close( );
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            SUTL_SETUP_ASSERT(!t.GetFileStream( ).is_open( ));

            t.SetTargetFile(GetGoodFileString( ));

            try
            {
                t.InitializeFileStream( );
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }
    }

    namespace RestoreFileStream
    {
        /// Negative Tests \\\

        UnitTestResult NoFile( )
        {
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            t.SetTargetFile(L"");

            try
            {
                SUTL_TEST_ASSERT(!t.RestoreFileStream( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(!t.GetFileStream( ).is_open( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult BadPath( )
        {
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            t.SetTargetFile(GetBadFileString( ));

            try
            {
                SUTL_TEST_ASSERT(!t.RestoreFileStream( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(!t.GetFileStream( ).is_open( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        /// Positive Tests \\\

        UnitTestResult GoodPath( )
        {
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            t.SetTargetFile(GetGoodFileString( ));

            try
            {
                SUTL_TEST_ASSERT(t.RestoreFileStream( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }
    }

    namespace Flush
    {
        /// Negative Tests \\\

        UnitTestResult NoFile( )
        {
            bool threw = false;
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            t.SetTargetFile(L"");

            try
            {
                t.GetFileStream( ).close( );
                t.InitializeFileStream( );
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_SETUP_ASSERT(threw);
            SUTL_SETUP_ASSERT(t.GetFileStream( ).good( ));
            SUTL_SETUP_ASSERT(!t.GetFileStream( ).is_open( ));

            try
            {
                t.GetLogger( ).Flush( );
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
            SUTL_TEST_ASSERT(!t.GetFileStream( ).is_open( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult BadStream( )
        {
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            for ( auto state : std::vector<int>{ std::ios_base::failbit, std::ios_base::badbit, std::ios_base::failbit | std::ios_base::badbit } )
            {
                t.GetFileStream( ).clear( );
                t.GetFileStream( ).setstate(state);

                try
                {
                    t.GetLogger( ).Flush( );
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }

                SUTL_TEST_ASSERT(t.GetFileStream( ).rdstate( ) == state);
                SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
            }

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        /// Positive Tests \\\

        UnitTestResult NoContent( )
        {
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                t.GetLogger( ).Flush( );
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult GoodStream( )
        {
            const std::wstring testStr = L"Test string";
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                t.GetFileStream( ) << testStr;
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                t.GetLogger( ).Flush( );
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
            SUTL_TEST_ASSERT(ReadTestFile( ) == testStr);

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }
    }

    namespace LogPrefixes
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult NoFile( )
        {
            bool threw = false;
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            t.SetTargetFile(L"");

            try
            {
                t.GetFileStream( ).close( );
                t.InitializeFileStream( );
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_SETUP_ASSERT(threw);
            SUTL_SETUP_ASSERT(t.GetFileStream( ).good( ));
            SUTL_SETUP_ASSERT(!t.GetFileStream( ).is_open( ));

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; INCREMENT_OPTIONFLAG(mask) )
            {
                t.GetConfig( ).Disable(OptionFlag::PREFIX_MASK);
                t.GetConfig( ).Enable(mask);

                threw = false;

                try
                {
                    t.LogPrefixes<T>(VerbosityLevel::INFO, std::this_thread::get_id( ));
                }
                catch ( const std::exception& )
                {
                    threw = true;
                }

                SUTL_TEST_ASSERT(threw);
                SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
                SUTL_TEST_ASSERT(!t.GetFileStream( ).is_open( ));
            }

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadStream( )
        {
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; INCREMENT_OPTIONFLAG(mask) )
            {
                bool threw = false;
                
                t.GetConfig( ).Disable(OptionFlag::PREFIX_MASK);
                t.GetConfig( ).Enable(mask);

                t.GetFileStream( ).setstate(std::ios_base::badbit);

                try
                {
                    t.LogPrefixes<T>(VerbosityLevel::INFO, std::this_thread::get_id( ));
                }
                catch ( const std::exception& )
                {
                    threw = true;
                }

                SUTL_TEST_ASSERT(threw);
                SUTL_TEST_ASSERT(!t.GetFileStream( ).good( ));
                SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
            }

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadVerbosityLevel( )
        {
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; INCREMENT_OPTIONFLAG(mask) )
            {
                t.GetConfig( ).Disable(OptionFlag::PREFIX_MASK);
                t.GetConfig( ).Enable(mask);

                try
                {
                    t.LogPrefixes<T>(VerbosityLevel::MAX, std::this_thread::get_id( ));
                    t.GetFileStream( ).flush( );
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }

                SUTL_TEST_ASSERT(ReadTestFile( ).empty( ));
                SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
                SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
            }

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        /// Positive Tests \\\

        template <class T>
        UnitTestResult GoodStream( )
        {
            Tester t;

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(static_cast<SLL::OptionFlagType>(mask) + static_cast<SLL::OptionFlagType>(OptionFlag::PREFIX_BEGIN)) )
            {
                FILE_LOGGER_TEST_COMMON_SETUP(t);

                t.GetConfig( ).Enable(mask);

                try
                {
                    t.LogPrefixes<T>(VerbosityLevel::INFO, std::this_thread::get_id( ));
                    t.GetFileStream( ).flush( );
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }

                SUTL_TEST_ASSERT(ValidatePrefixes(t.GetConfig( ), ReadTestFile( )));
                SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
                SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));

                FILE_LOGGER_TEST_COMMON_CLEANUP(t);
            }

            SUTL_TEST_SUCCESS( );
        }
    }

    namespace LogMessage
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult NoFile( )
        {
            std::unique_ptr<T[ ]> pFormat;
            bool threw = false;
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #%d\n");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            t.GetFileStream( ).close( );
            t.SetTargetFile(L"");

            try
            {
                t.InitializeFileStream( );
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_SETUP_ASSERT(threw);
            SUTL_SETUP_ASSERT(!t.GetFileStream( ).is_open( ));
            SUTL_SETUP_ASSERT(t.GetFileStream( ).good( ));

            threw = false;
            try
            {
                t.LogMessage<T>(pFormat.get( ), 1);
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_TEST_ASSERT(threw);
            SUTL_TEST_ASSERT(!t.GetFileStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadStream( )
        {
            std::unique_ptr<T[ ]> pFormat;
            bool threw = false;
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #%d\n");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            t.GetFileStream( ).setstate(std::ios_base::badbit);

            threw = false;
            try
            {
                t.LogMessage<T>(pFormat.get( ), 1);
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_TEST_ASSERT(threw);
            SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
            SUTL_TEST_ASSERT(!t.GetFileStream( ).good( ));
            SUTL_TEST_ASSERT(ReadTestFile( ).empty( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadFormat( )
        {
            std::unique_ptr<T[ ]> pFormat;
            bool threw = false;
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            threw = false;
            try
            {
                t.LogMessage<T>(pFormat.get( ), 1);
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_TEST_ASSERT(threw);
            SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
            SUTL_TEST_ASSERT(ReadTestFile( ).empty( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        /// Positive Tests \\\
                
        template <class T>
        UnitTestResult ZeroLengthFormat( )
        {
            std::unique_ptr<T[ ]> pFormat;
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                t.LogMessage<T>(pFormat.get( ), 1);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
            SUTL_TEST_ASSERT(ReadTestFile( ).empty( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult PlainFormat( )
        {
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #1\r\n");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string #1");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                t.LogMessage<T>(pFormat.get( ));
                t.GetFileStream( ).flush( );
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
            SUTL_TEST_ASSERT(ReadTestFile( ).compare(pExpected.get( )) == 0);

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult IntegralFormat( )
        {
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string, integer specifiers %d %llu.\r\n");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string, integer specifiers -500 300000.");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                t.LogMessage<T>(pFormat.get( ), -500, 300000ull);
                t.GetFileStream( ).flush( );
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
            SUTL_TEST_ASSERT(ReadTestFile( ).compare(pExpected.get( )) == 0);

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult FloatingFormat( )
        {
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string, floating-point specifiers %2.2f %1.5f.\r\n");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string, floating-point specifiers -35.02 1.57779.");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                t.LogMessage<T>(pFormat.get( ), -35.019, 1.577789);
                t.GetFileStream( ).flush( );
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
            SUTL_TEST_ASSERT(ReadTestFile( ).compare(pExpected.get( )) == 0);

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult StringFormat( )
        {
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;
            std::unique_ptr<T[ ]> arg1;

            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string, string specifiers %s %ls.\r\n");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string, string specifiers Test string #1 \"Test %f %g string %p #2\".");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                arg1 = StringUtil::ConvertAndCopy<T>("Test string #1");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                t.LogMessage<T>(pFormat.get( ), arg1.get( ), L"\"Test %f %g string %p #2\"");
                t.GetFileStream( ).flush( );
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
            SUTL_TEST_ASSERT(ReadTestFile( ).compare(pExpected.get( )) == 0);

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }
    }

    namespace Log
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult BadVerbosityLevel( )
        {
            std::unique_ptr<T[ ]> pFormat;
            bool threw = false;
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #%d\n");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                t.GetLogger( ).Log<T>(VerbosityLevel::MAX, pFormat.get( ), 1);
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_TEST_ASSERT(threw);
            SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadFormat( )
        {
            std::unique_ptr<T[ ]> pFormat;
            bool threw = false;
            Tester t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                t.GetLogger( ).Log<T>(VerbosityLevel::INFO, pFormat.get( ), 1);
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_TEST_ASSERT(threw);
            SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        /// Positive Tests \\\

        template <class T>
        UnitTestResult ZeroLengthFormat( )
        {
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;
            Tester t;

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(static_cast<SLL::OptionFlagType>(mask) + static_cast<SLL::OptionFlagType>(OptionFlag::PREFIX_BEGIN)) )
            {
                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    FILE_LOGGER_TEST_COMMON_SETUP(t);

                    t.GetConfig( ).Enable(mask);

                    try
                    {
                        t.GetFileStream( ).close( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_SETUP_EXCEPTION(e.what( ));
                    }

                    try
                    {
                        t.GetLogger( ).Log<T>(lvl, pFormat.get( ));
                        t.GetFileStream( ).flush( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
                    SUTL_TEST_ASSERT(ValidateLog(t.GetConfig( ), ReadTestFile( ), pExpected));

                    FILE_LOGGER_TEST_COMMON_CLEANUP(t);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult PlainFormat( )
        {
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;
            Tester t;

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #1\r\n");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string #1");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(static_cast<SLL::OptionFlagType>(mask) + static_cast<SLL::OptionFlagType>(OptionFlag::PREFIX_BEGIN)) )
            {
                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    FILE_LOGGER_TEST_COMMON_SETUP(t);

                    t.GetConfig( ).Enable(mask);

                    try
                    {
                        t.GetLogger( ).Log<T>(lvl, pFormat.get( ));
                        t.GetFileStream( ).flush( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
                    SUTL_TEST_ASSERT(ValidateLog(t.GetConfig( ), ReadTestFile( ), pExpected));

                    FILE_LOGGER_TEST_COMMON_CLEANUP(t);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult IntegralFormat( )
        {
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;
            Tester t;

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string, integer specifiers %d %llu.\r\n");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string, integer specifiers -500 300000.");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(static_cast<SLL::OptionFlagType>(mask) + static_cast<SLL::OptionFlagType>(OptionFlag::PREFIX_BEGIN)) )
            {
                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    FILE_LOGGER_TEST_COMMON_SETUP(t);

                    t.GetConfig( ).Enable(mask);

                    try
                    {
                        t.GetLogger( ).Log<T>(lvl, pFormat.get( ), -500, 300000ull);
                        t.GetFileStream( ).flush( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
                    SUTL_TEST_ASSERT(ValidateLog(t.GetConfig( ), ReadTestFile( ), pExpected));

                    FILE_LOGGER_TEST_COMMON_CLEANUP(t);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult FloatingFormat( )
        {
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;
            Tester t;

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string, floating-point specifiers %2.2f %1.5f.\r\n");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string, floating-point specifiers -35.02 1.57779.");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(static_cast<SLL::OptionFlagType>(mask) + static_cast<SLL::OptionFlagType>(OptionFlag::PREFIX_BEGIN)) )
            {
                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    FILE_LOGGER_TEST_COMMON_SETUP(t);

                    t.GetConfig( ).Enable(mask);

                    try
                    {
                        t.GetLogger( ).Log<T>(lvl, pFormat.get( ), -35.019, 1.577789);
                        t.GetFileStream( ).flush( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
                    SUTL_TEST_ASSERT(ValidateLog(t.GetConfig( ), ReadTestFile( ), pExpected));

                    FILE_LOGGER_TEST_COMMON_CLEANUP(t);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult StringFormat( )
        {
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;

            std::unique_ptr<T[ ]> arg1;
            const wchar_t* arg2 = L"\"Test %f %g string %p #2\"";

            Tester t;

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string, string specifiers %s %ls.\r\n");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string, string specifiers Test string #1 \"Test %f %g string %p #2\".");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                arg1 = StringUtil::ConvertAndCopy<T>("Test string #1");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(static_cast<SLL::OptionFlagType>(mask) + static_cast<SLL::OptionFlagType>(OptionFlag::PREFIX_BEGIN)) )
            {
                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    FILE_LOGGER_TEST_COMMON_SETUP(t);

                    t.GetConfig( ).Enable(mask);

                    try
                    {
                        t.GetLogger( ).Log<T>(lvl, pFormat.get( ), arg1.get( ), arg2);
                        t.GetFileStream( ).flush( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
                    SUTL_TEST_ASSERT(ValidateLog(t.GetConfig( ), ReadTestFile( ), pExpected));

                    FILE_LOGGER_TEST_COMMON_CLEANUP(t);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult RecoverFromNotOpen( )
        {
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;
            Tester t;

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #%d\n");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string #1");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(static_cast<SLL::OptionFlagType>(mask) + static_cast<SLL::OptionFlagType>(OptionFlag::PREFIX_BEGIN)) )
            {
                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    FILE_LOGGER_TEST_COMMON_SETUP(t);

                    t.GetConfig( ).Enable(mask);

                    try
                    {
                        t.GetFileStream( ).close( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_SETUP_EXCEPTION(e.what( ));
                    }

                    try
                    {
                        t.GetLogger( ).Log<T>(lvl, pFormat.get( ), 1);
                        t.GetFileStream( ).flush( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
                    SUTL_TEST_ASSERT(ValidateLog(t.GetConfig( ), ReadTestFile( ), pExpected));

                    FILE_LOGGER_TEST_COMMON_CLEANUP(t);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult RecoverFromBadState( )
        {
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;
            Tester t;

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #%d\n");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string #1");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(static_cast<SLL::OptionFlagType>(mask) + static_cast<SLL::OptionFlagType>(OptionFlag::PREFIX_BEGIN)) )
            {
                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    FILE_LOGGER_TEST_COMMON_SETUP(t);

                    t.GetConfig( ).Enable(mask);
                    t.GetFileStream( ).setstate(std::ios_base::badbit);

                    try
                    {
                        t.GetLogger( ).Log<T>(lvl, pFormat.get( ), 1);
                        t.GetFileStream( ).flush( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));
                    SUTL_TEST_ASSERT(ValidateLog(t.GetConfig( ), ReadTestFile( ), pExpected));

                    FILE_LOGGER_TEST_COMMON_CLEANUP(t);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult NoLogBelowThreshold( )
        {
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;
            Tester t;

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #%d\n");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string #1");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(static_cast<SLL::OptionFlagType>(mask) + static_cast<SLL::OptionFlagType>(OptionFlag::PREFIX_BEGIN)) )
            {
                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    for ( VerbosityLevel threshold = VerbosityLevel::BEGIN; threshold < VerbosityLevel::MAX; INCREMENT_VERBOSITY(threshold) )
                    {
                        FILE_LOGGER_TEST_COMMON_SETUP(t);

                        t.GetConfig( ).Enable(mask);
                        t.GetConfig( ).SetVerbosityThreshold(threshold);

                        try
                        {
                            t.GetLogger( ).Log<T>(lvl, pFormat.get( ), 1);
                            t.GetFileStream( ).flush( );
                        }
                        catch ( const std::exception& e )
                        {
                            SUTL_TEST_EXCEPTION(e.what( ));
                        }

                        SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
                        SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));

                        if ( lvl < threshold )
                        {
                            SUTL_TEST_ASSERT(ReadTestFile( ).empty( ));
                        }
                        else
                        {
                            SUTL_TEST_ASSERT(ValidateLog(t.GetConfig( ), ReadTestFile( ), pExpected));
                        }

                        FILE_LOGGER_TEST_COMMON_CLEANUP(t);
                    }
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult FlushImportantMessage( )
        {
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;
            Tester t;

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #%d\n");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string #1");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(static_cast<SLL::OptionFlagType>(mask) + static_cast<SLL::OptionFlagType>(OptionFlag::PREFIX_BEGIN)) )
            {
                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    FILE_LOGGER_TEST_COMMON_SETUP(t);

                    t.GetConfig( ).Enable(mask);
                    t.GetFileStream( ).setstate(std::ios_base::badbit);

                    try
                    {
                        t.GetLogger( ).Log<T>(lvl, pFormat.get( ), 1);
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));

                    if ( lvl <= VerbosityLevel::INFO )
                    {
                        SUTL_TEST_ASSERT(ReadTestFile( ).empty( ));
                    }
                    else
                    {
                        SUTL_TEST_ASSERT(ValidateLog(t.GetConfig( ), ReadTestFile( ), pExpected));
                    }

                    FILE_LOGGER_TEST_COMMON_CLEANUP(t);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult AppendFileWrite( )
        {
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected1;
            std::unique_ptr<wchar_t[ ]> pExpected2;

            Tester t;

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #%d\r\n");
                pExpected1 = StringUtil::ConvertAndCopy<wchar_t>("Test string #1");
                pExpected2 = StringUtil::ConvertAndCopy<wchar_t>("Test string #2");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(static_cast<SLL::OptionFlagType>(mask) + static_cast<SLL::OptionFlagType>(OptionFlag::PREFIX_BEGIN)) )
            {
                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    std::wstring fileContents;
                    std::wstring line1;
                    std::wstring line2;
                    size_t pos = 0;

                    FILE_LOGGER_TEST_COMMON_SETUP(t);

                    t.GetConfig( ).Enable(mask);

                    try
                    {
                        t.GetLogger( ).Log<T>(lvl, pFormat.get( ), 1);
                        t.GetLogger( ).Log<T>(lvl, pFormat.get( ), 2);
                        t.GetFileStream( ).flush( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(t.GetFileStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetFileStream( ).is_open( ));

                    fileContents = ReadTestFile( );
                    SUTL_TEST_ASSERT(!fileContents.empty( ));

                    pos = fileContents.find(L'\n');
                    SUTL_TEST_ASSERT(pos != std::wstring::npos);

                    line1 = fileContents.substr(0, pos);
                    line2 = fileContents.substr(pos + 1);

                    SUTL_TEST_ASSERT(ValidateLog(t.GetConfig( ), line1, pExpected1));
                    SUTL_TEST_ASSERT(ValidateLog(t.GetConfig( ), line2, pExpected2));

                    FILE_LOGGER_TEST_COMMON_CLEANUP(t);
                }
            }

            SUTL_TEST_SUCCESS( );
        }
    }
}