// Target Test Declarations
#include <FileLoggerTests.h>

namespace FileLoggerTests
{
    using SLL::ConfigPackage;
    using SLL::FileLogger;
    using SLL::FileStream;
    using SLL::OptionFlag;
    using SLL::VerbosityLevel;

    std::list<std::function<UnitTestResult(void)>> GetTests( )
    {
        static const std::list<std::function<UnitTestResult(void)>> testList
        {
            /// InitializeStream Tests \\\

            // Negative Tests
            InitializeStream::NoFile,
            InitializeStream::BadPath,

            // Positive Test
            InitializeStream::GoodPath,

            /// RestoreStream Tests \\\

            // Negative Tests
            RestoreStream::NoFile,
            RestoreStream::BadPath,

            // Positive Test
            RestoreStream::GoodPath,

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
            Log::NoFile<char>,
            Log::NoFile<wchar_t>,

            Log::BadStream<char>,
            Log::BadStream<wchar_t>,

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

    typedef StreamLoggerTests::Tester<FileLogger, FileStream> TesterHelper;

    /// Helper Macros \\\

// Perform setup common to most unit tests.
//  - Build new file logger, target (hopefully) good log test-file.
//  - Setup-asserts to ensure target file is open and file stream is in a good state.
#define FILE_LOGGER_TEST_COMMON_SETUP(t)                                                \
    try                                                                                 \
    {                                                                                   \
        t.SetLogger(BuildConfig(GetGoodFileString( ), VerbosityLevel::INFO));           \
    }                                                                                   \
    catch ( const std::exception& e )                                                   \
    {                                                                                   \
        SUTL_SETUP_EXCEPTION(e.what( ));                                                \
    }                                                                                   \
                                                                                        \
    SUTL_SETUP_ASSERT(!t.GetConfig( ).GetFile( ).empty( ));                             \
    SUTL_SETUP_ASSERT(t.GetStream( ).good( ));                                          \
    SUTL_SETUP_ASSERT(t.GetStream( ).is_open( ));


// Perform cleanup common to most unit tests.
//  - Close file stream.
//  - Attempt to delete the test-log file.
#define FILE_LOGGER_TEST_COMMON_CLEANUP(t)      \
    try                                         \
    {                                           \
        t.GetStream( ).close( );                \
        SUTL_CLEANUP_ASSERT(DeleteTestFile( )); \
    }                                           \
    catch ( const std::exception& e )           \
    {                                           \
        SUTL_CLEANUP_EXCEPTION(e.what( ));      \
    }


    /// Unit Tests \\\

    namespace InitializeStream
    {
        /// Negative Tests \\\

        UnitTestResult NoFile( )
        {
            bool threw = false;
            TesterHelper t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                t.GetStream( ).close( );
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            SUTL_SETUP_ASSERT(!t.GetStream( ).is_open( ));

            t.GetConfig( ).SetFile<wchar_t>(L"");

            try
            {
                t.InitializeStream( );
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_TEST_ASSERT(threw);
            SUTL_TEST_ASSERT(!t.GetStream( ).is_open( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult BadPath( )
        {
            bool threw = false;
            TesterHelper t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                t.GetStream( ).close( );
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            SUTL_SETUP_ASSERT(!t.GetStream( ).is_open( ));

            t.GetConfig( ).SetFile(GetBadFileString( ));

            try
            {
                t.InitializeStream( );
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_TEST_ASSERT(threw);
            SUTL_TEST_ASSERT(!t.GetStream( ).is_open( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        /// Positive Tests \\\

        UnitTestResult GoodPath( )
        {
            TesterHelper t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                t.GetStream( ).close( );
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            SUTL_SETUP_ASSERT(!t.GetStream( ).is_open( ));

            t.GetConfig( ).SetFile(GetGoodFileString( ));

            try
            {
                t.InitializeStream( );
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }
    }

    namespace RestoreStream
    {
        /// Negative Tests \\\

        UnitTestResult NoFile( )
        {
            TesterHelper t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            t.GetConfig( ).SetFile<wchar_t>(L"");

            try
            {
                SUTL_TEST_ASSERT(!t.RestoreStream( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(!t.GetStream( ).is_open( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult BadPath( )
        {
            TesterHelper t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            t.GetConfig( ).SetFile(GetBadFileString( ));

            try
            {
                SUTL_TEST_ASSERT(!t.RestoreStream( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(!t.GetStream( ).is_open( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        /// Positive Tests \\\

        UnitTestResult GoodPath( )
        {
            TesterHelper t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            t.GetConfig( ).SetFile(GetGoodFileString( ));

            try
            {
                SUTL_TEST_ASSERT(t.RestoreStream( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));

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
            TesterHelper t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            t.GetConfig( ).SetFile<wchar_t>(L"");

            try
            {
                t.GetStream( ).close( );
                t.InitializeStream( );
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_SETUP_ASSERT(threw);
            SUTL_SETUP_ASSERT(t.GetStream( ).good( ));
            SUTL_SETUP_ASSERT(!t.GetStream( ).is_open( ));

            try
            {
                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    t.Flush(lvl);
                }
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(!t.GetStream( ).is_open( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult BadStream( )
        {
            TesterHelper t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            for ( auto state : std::vector<int>{ std::ios_base::failbit, std::ios_base::badbit, std::ios_base::failbit | std::ios_base::badbit } )
            {
                t.GetStream( ).clear( );
                t.GetStream( ).setstate(state);

                try
                {
                    for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                    {
                        t.Flush(lvl);

                        SUTL_TEST_ASSERT(t.GetStream( ).rdstate( ) == state);
                        SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
                    }
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }

                SUTL_TEST_ASSERT(t.GetStream( ).rdstate( ) == state);
                SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
            }

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        /// Positive Tests \\\

        UnitTestResult NoContent( )
        {
            TesterHelper t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                t.Flush(VerbosityLevel::FATAL);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult GoodStream( )
        {
            const std::wstring testStr = L"Test string";
            TesterHelper t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                t.GetStream( ) << testStr;
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                t.Flush(VerbosityLevel::FATAL);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
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
            TesterHelper t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            t.GetConfig( ).SetFile<wchar_t>(L"");

            try
            {
                t.GetStream( ).close( );
                t.InitializeStream( );
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_SETUP_ASSERT(threw);
            SUTL_SETUP_ASSERT(t.GetStream( ).good( ));
            SUTL_SETUP_ASSERT(!t.GetStream( ).is_open( ));

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
                SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                SUTL_TEST_ASSERT(!t.GetStream( ).is_open( ));
            }

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadStream( )
        {
            TesterHelper t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; INCREMENT_OPTIONFLAG(mask) )
            {
                bool threw = false;
                
                t.GetConfig( ).Disable(OptionFlag::PREFIX_MASK);
                t.GetConfig( ).Enable(mask);

                t.GetStream( ).setstate(std::ios_base::badbit);

                try
                {
                    t.LogPrefixes<T>(VerbosityLevel::INFO, std::this_thread::get_id( ));
                }
                catch ( const std::exception& )
                {
                    threw = true;
                }

                SUTL_TEST_ASSERT(threw);
                SUTL_TEST_ASSERT(!t.GetStream( ).good( ));
                SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
            }

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadVerbosityLevel( )
        {
            TesterHelper t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; INCREMENT_OPTIONFLAG(mask) )
            {
                t.GetConfig( ).Disable(OptionFlag::PREFIX_MASK);
                t.GetConfig( ).Enable(mask);

                try
                {
                    t.LogPrefixes<T>(VerbosityLevel::MAX, std::this_thread::get_id( ));
                    t.GetStream( ).flush( );
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }

                SUTL_TEST_ASSERT(ReadTestFile( ).empty( ));
                SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
            }

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        /// Positive Tests \\\

        template <class T>
        UnitTestResult GoodStream( )
        {
            TesterHelper t;

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(static_cast<SLL::OptionFlagType>(mask) + static_cast<SLL::OptionFlagType>(OptionFlag::PREFIX_BEGIN)) )
            {
                FILE_LOGGER_TEST_COMMON_SETUP(t);

                t.GetConfig( ).Enable(mask);

                try
                {
                    t.LogPrefixes<T>(VerbosityLevel::INFO, std::this_thread::get_id( ));
                    t.GetStream( ).flush( );
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }

                SUTL_TEST_ASSERT(StreamLoggerTests::ValidatePrefixes(t.GetConfig( ), ReadTestFile( )));
                SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));

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
            TesterHelper t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #%d\n");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            t.GetStream( ).close( );
            t.GetConfig( ).SetFile<wchar_t>(L"");

            try
            {
                t.InitializeStream( );
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_SETUP_ASSERT(threw);
            SUTL_SETUP_ASSERT(!t.GetStream( ).is_open( ));
            SUTL_SETUP_ASSERT(t.GetStream( ).good( ));

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
            SUTL_TEST_ASSERT(!t.GetStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetStream( ).good( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadStream( )
        {
            std::unique_ptr<T[ ]> pFormat;
            bool threw = false;
            TesterHelper t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #%d\n");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            t.GetStream( ).setstate(std::ios_base::badbit);

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
            SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
            SUTL_TEST_ASSERT(!t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(ReadTestFile( ).empty( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadFormat( )
        {
            std::unique_ptr<T[ ]> pFormat;
            bool threw = false;
            TesterHelper t;

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
            SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(ReadTestFile( ).empty( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        /// Positive Tests \\\
                
        template <class T>
        UnitTestResult ZeroLengthFormat( )
        {
            std::unique_ptr<T[ ]> pFormat;
            TesterHelper t;

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

            SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(ReadTestFile( ).empty( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult PlainFormat( )
        {
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;
            TesterHelper t;

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
                t.GetStream( ).flush( );
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(ReadTestFile( ).compare(pExpected.get( )) == 0);

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult IntegralFormat( )
        {
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;
            TesterHelper t;

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
                t.GetStream( ).flush( );
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(ReadTestFile( ).compare(pExpected.get( )) == 0);

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult FloatingFormat( )
        {
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;
            TesterHelper t;

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
                t.GetStream( ).flush( );
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
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

            TesterHelper t;

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
                t.GetStream( ).flush( );
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(ReadTestFile( ).compare(pExpected.get( )) == 0);

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }
    }

    namespace Log
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult NoFile( )
        {
            std::unique_ptr<T[ ]> pFormat;
            bool ret = false;
            bool threw = false;
            TesterHelper t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #%d\n");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            t.GetStream( ).close( );
            t.GetConfig( ).SetFile<wchar_t>(L"");

            try
            {
                t.InitializeStream( );
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_SETUP_ASSERT(threw);
            SUTL_SETUP_ASSERT(!t.GetStream( ).is_open( ));
            SUTL_SETUP_ASSERT(t.GetStream( ).good( ));

            try
            {
                ret = t.GetLogger( ).Log<T>(VerbosityLevel::INFO, pFormat.get( ), 1);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }            

            SUTL_TEST_ASSERT(!ret);
            SUTL_TEST_ASSERT(!t.GetStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetStream( ).good( ));

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadStream( )
        {
            std::unique_ptr<T[ ]> pFormat;
            bool ret = false;
            TesterHelper t;

            FILE_LOGGER_TEST_COMMON_SETUP(t);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #%d\n");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            t.GetStream( ).setstate(std::ios_base::badbit);

            try
            {
                ret = t.GetLogger( ).Log<T>(VerbosityLevel::INFO, pFormat.get( ), 1);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(ret);
            SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetStream( ).good( ));

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadVerbosityLevel( )
        {
            std::unique_ptr<T[ ]> pFormat;
            bool threw = false;
            TesterHelper t;

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
            SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetStream( ).good( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadFormat( )
        {
            std::unique_ptr<T[ ]> pFormat;
            bool threw = false;
            TesterHelper t;

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
            SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
            SUTL_TEST_ASSERT(t.GetStream( ).good( ));

            FILE_LOGGER_TEST_COMMON_CLEANUP(t);

            SUTL_TEST_SUCCESS( );
        }

        /// Positive Tests \\\

        template <class T>
        UnitTestResult ZeroLengthFormat( )
        {
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;
            TesterHelper t;

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
                    bool ret = false;

                    FILE_LOGGER_TEST_COMMON_SETUP(t);

                    t.GetConfig( ).Enable(mask);

                    try
                    {
                        t.GetStream( ).close( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_SETUP_EXCEPTION(e.what( ));
                    }

                    try
                    {
                        ret = t.GetLogger( ).Log<T>(lvl, pFormat.get( ));
                        t.GetStream( ).flush( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog(t.GetConfig( ), ReadTestFile( ), pExpected));

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
            TesterHelper t;

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
                    bool ret = false;

                    FILE_LOGGER_TEST_COMMON_SETUP(t);

                    t.GetConfig( ).Enable(mask);

                    try
                    {
                        ret = t.GetLogger( ).Log<T>(lvl, pFormat.get( ));
                        t.GetStream( ).flush( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog(t.GetConfig( ), ReadTestFile( ), pExpected));

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
            TesterHelper t;

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
                    bool ret = false;

                    FILE_LOGGER_TEST_COMMON_SETUP(t);

                    t.GetConfig( ).Enable(mask);

                    try
                    {
                        ret = t.GetLogger( ).Log<T>(lvl, pFormat.get( ), -500, 300000ull);
                        t.GetStream( ).flush( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog(t.GetConfig( ), ReadTestFile( ), pExpected));

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
            TesterHelper t;

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
                    bool ret = false;

                    FILE_LOGGER_TEST_COMMON_SETUP(t);

                    t.GetConfig( ).Enable(mask);

                    try
                    {
                        ret = t.GetLogger( ).Log<T>(lvl, pFormat.get( ), -35.019, 1.577789);
                        t.GetStream( ).flush( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog(t.GetConfig( ), ReadTestFile( ), pExpected));

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

            TesterHelper t;

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
                    bool ret = false;

                    FILE_LOGGER_TEST_COMMON_SETUP(t);

                    t.GetConfig( ).Enable(mask);

                    try
                    {
                        ret = t.GetLogger( ).Log<T>(lvl, pFormat.get( ), arg1.get( ), arg2);
                        t.GetStream( ).flush( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog(t.GetConfig( ), ReadTestFile( ), pExpected));

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
            TesterHelper t;

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
                    bool ret = false;

                    FILE_LOGGER_TEST_COMMON_SETUP(t);

                    t.GetConfig( ).Enable(mask);

                    try
                    {
                        t.GetStream( ).close( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_SETUP_EXCEPTION(e.what( ));
                    }

                    try
                    {
                        ret = t.GetLogger( ).Log<T>(lvl, pFormat.get( ), 1);
                        t.GetStream( ).flush( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog(t.GetConfig( ), ReadTestFile( ), pExpected));

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
            TesterHelper t;

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
                    bool ret = false;

                    FILE_LOGGER_TEST_COMMON_SETUP(t);

                    t.GetConfig( ).Enable(mask);
                    t.GetStream( ).setstate(std::ios_base::badbit);

                    try
                    {
                        ret = t.GetLogger( ).Log<T>(lvl, pFormat.get( ), 1);
                        t.GetStream( ).flush( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog(t.GetConfig( ), ReadTestFile( ), pExpected));

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
            TesterHelper t;

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
                        bool ret = false;

                        FILE_LOGGER_TEST_COMMON_SETUP(t);

                        t.GetConfig( ).Enable(mask);
                        t.GetConfig( ).SetVerbosityThreshold(threshold);

                        try
                        {
                            ret = t.GetLogger( ).Log<T>(lvl, pFormat.get( ), 1);
                            t.GetStream( ).flush( );
                        }
                        catch ( const std::exception& e )
                        {
                            SUTL_TEST_EXCEPTION(e.what( ));
                        }

                        SUTL_TEST_ASSERT(ret);
                        SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                        SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));

                        if ( lvl < threshold )
                        {
                            SUTL_TEST_ASSERT(ReadTestFile( ).empty( ));
                        }
                        else
                        {
                            SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog(t.GetConfig( ), ReadTestFile( ), pExpected));
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
            TesterHelper t;

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
                    bool ret = false;

                    FILE_LOGGER_TEST_COMMON_SETUP(t);

                    t.GetConfig( ).Enable(mask);
                    t.GetStream( ).setstate(std::ios_base::badbit);

                    try
                    {
                        ret = t.GetLogger( ).Log<T>(lvl, pFormat.get( ), 1);
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));

                    if ( lvl >= VerbosityLevel::WARN )
                    {
                        SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog(t.GetConfig( ), ReadTestFile( ), pExpected));
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

            TesterHelper t;

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
                    bool ret1 = false;
                    bool ret2 = false;

                    std::wstring fileContents;
                    std::wstring line1;
                    std::wstring line2;
                    size_t pos = 0;

                    FILE_LOGGER_TEST_COMMON_SETUP(t);

                    t.GetConfig( ).Enable(mask);

                    try
                    {
                        ret1 = t.GetLogger( ).Log<T>(lvl, pFormat.get( ), 1);
                        ret2 = t.GetLogger( ).Log<T>(lvl, pFormat.get( ), 2);
                        t.GetStream( ).flush( );
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret1);
                    SUTL_TEST_ASSERT(ret2);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).is_open( ));

                    fileContents = ReadTestFile( );
                    SUTL_TEST_ASSERT(!fileContents.empty( ));

                    pos = fileContents.find(L'\n');
                    SUTL_TEST_ASSERT(pos != std::wstring::npos);

                    line1 = fileContents.substr(0, pos);
                    line2 = fileContents.substr(pos + 1);

                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog(t.GetConfig( ), line1, pExpected1));
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog(t.GetConfig( ), line2, pExpected2));

                    FILE_LOGGER_TEST_COMMON_CLEANUP(t);
                }
            }

            SUTL_TEST_SUCCESS( );
        }
    }
}