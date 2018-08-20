// For StreamLogger Tester class.
#include <FileLoggerTests.h>

// Test declarations.
#include <StdOutLoggerTests.h>

#include <locale>

namespace StdOutLoggerTests
{
    using SLL::Color;
    using SLL::ConfigPackage;
    using SLL::StdOutLogger;
    using SLL::StdOutStream;
    using SLL::OptionFlag;
    using SLL::VerbosityLevel;

    std::list<std::function<UnitTestResult(void)>> GetTests( )
    {
        static std::list<std::function<UnitTestResult(void)>> testList
        {
            // InitializeStream Tests

            /// Negative Tests \\\

            InitializeStream::NoBuffer,
            InitializeStream::BadStream,

            /// Positive Tests \\\

            InitializeStream::GoodStream,

            // RestoreStream Tests

            /// Positive Tests \\\

            RestoreStream::NoBuffer,
            RestoreStream::BadStream,
            RestoreStream::GoodStream,

            // Flush Tests

            /// Negative Tests \\\

            Flush::NoBuffer,
            Flush::BadStream,

            /// Positive Tests \\\

            Flush::NoContent,

            // LogPrefixes Tests

            /// Negative Tests \\\

            LogPrefixes::NoBuffer<char>,
            LogPrefixes::NoBuffer<wchar_t>,

            LogPrefixes::BadStream<char>,
            LogPrefixes::BadStream<wchar_t>,

            LogPrefixes::BadVerbosityLevel<char>,
            LogPrefixes::BadVerbosityLevel<wchar_t>,

            /// Positive Tests \\\

            LogPrefixes::GoodStream<char>,
            LogPrefixes::GoodStream<wchar_t>,

            // LogMessage Tests

            /// Negative Tests \\\

            LogMessage::NoBuffer<char>,
            LogMessage::NoBuffer<wchar_t>,

            LogMessage::BadStream<char>,
            LogMessage::BadStream<wchar_t>,

            LogMessage::BadFormat<char>,
            LogMessage::BadFormat<wchar_t>,

            /// Positive Tests \\\
                                                
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

            // Log Tests

            /// Negative Tests \\\

            Log::NoBuffer<char>,
            Log::NoBuffer<wchar_t>,

            Log::BadStream<char>,
            Log::BadStream<wchar_t>,

            Log::BadVerbosityLevel<char>,
            Log::BadVerbosityLevel<wchar_t>,

            Log::BadFormat<char>,
            Log::BadFormat<wchar_t>,

            /// Positive Tests \\\

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

            Log::RecoverFromNoBuffer<char>,
            Log::RecoverFromNoBuffer<wchar_t>,

            Log::RecoverFromBadState<char>,
            Log::RecoverFromBadState<wchar_t>,

            Log::NoLogBelowThreshold<char>,
            Log::NoLogBelowThreshold<wchar_t>,

            Log::FlushImportantMessage<char>,
            Log::FlushImportantMessage<wchar_t>,

            Log::AppendStreamWrite<char>,
            Log::AppendStreamWrite<wchar_t>,

            Log::ColorLog<char>,
            Log::ColorLog<wchar_t>,
        };

        return testList;
    }

    typedef StreamLoggerTests::Tester<StdOutLogger, StdOutStream> TesterHelper;

    /// Helper Functions and Macros \\\

    // Perform setup common to most unit tests.
    //  - Build new stdout logger, assign stream external buffer.
    //  - Setup-asserts to ensure stream is in a good state.
#define STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf)         \
    try                                                 \
    {                                                   \
        t.SetLogger(BuildConfig(VerbosityLevel::INFO)); \
    }                                                   \
    catch ( const std::exception& e )                   \
    {                                                   \
        SUTL_SETUP_EXCEPTION(e.what( ));                \
    }                                                   \
                                                        \
    t.GetStream( ).set_rdbuf(&buf);                     \
    t.SetStreamBuffer(buf);                             \
                                                        \
    SUTL_SETUP_ASSERT(t.GetStream( ).good( ));          \
    SUTL_SETUP_ASSERT(t.GetStream( ).rdbuf( ));         \
    SUTL_SETUP_ASSERT(t.GetStream( ).rdbuf( ) == &buf);

    // Reset stream's buffer.
#define STDOUT_LOGGER_TEST_RESET_BUFFER(buf)    \
    buf = std::wstringbuf( )                    \


    // Return true if the buffer is completely filled with zero values, false otherwise.
    bool StreamBufferIsZeroed(const std::wstringbuf& buf)
    {
        for ( auto c : buf.str( ) )
        {
            if ( c != static_cast<wchar_t>('\0') )
            {
                return false;
            }
        }

        return true;
    }

    template <class T>
    bool CompareBufferWithString(const std::wstringbuf& buf, const std::basic_string<T>& str)
    {
        const std::basic_string<T> bufferString = StringUtil::ConvertAndCopy<T>(buf.str( ));

        if ( str.empty( ) )
        {
            return StreamBufferIsZeroed(buf);
        }

        if ( str.size( ) > bufferString.size( ) )
        {
            return false;
        }

        return (memcmp(bufferString.data( ), str.data( ), sizeof(T) * (str.size( ) + 1)) == 0);
    }


    namespace InitializeStream
    {
        /// Negative Tests \\\

        UnitTestResult NoBuffer( )
        {
            TesterHelper t;
            std::wstringbuf buf;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            t.GetStream( ).set_rdbuf(nullptr);

            try
            {
                t.InitializeStream( );
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult BadStream( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            bool threw = false;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            for ( auto state : std::vector<int>{ std::ios_base::failbit, std::ios_base::badbit, std::ios_base::failbit | std::ios_base::badbit } )
            {
                t.GetStream( ).clear( );
                t.GetStream( ).setstate(state);

                try
                {
                    t.InitializeStream( );
                }
                catch ( const std::exception& )
                {
                    threw = true;
                }

                SUTL_TEST_ASSERT(threw);
                SUTL_TEST_ASSERT(!t.GetStream( ).good( ));
                SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                SUTL_TEST_ASSERT(StreamBufferIsZeroed(buf));
            }

            SUTL_TEST_SUCCESS( );
        }

        /// Positive Tests \\\

        UnitTestResult GoodStream( )
        {
            TesterHelper t;
            std::wstringbuf buf;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                t.InitializeStream( );
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
            SUTL_TEST_ASSERT(StreamBufferIsZeroed(buf));

            SUTL_TEST_SUCCESS( );
        }
    }

    namespace RestoreStream
    {
        /// Positive Tests \\\

        UnitTestResult NoBuffer( )
        {
            TesterHelper t;
            std::wstringbuf buf;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            t.GetStream( ).set_rdbuf(nullptr);

            try
            {
                t.RestoreStream( );
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
            SUTL_TEST_ASSERT(StreamBufferIsZeroed(buf));

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult BadStream( )
        {
            TesterHelper t;
            std::wstringbuf buf;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            for ( auto state : std::vector<int>{ std::ios_base::failbit, std::ios_base::badbit, std::ios_base::failbit | std::ios_base::badbit } )
            {
                t.GetStream( ).clear( );
                t.GetStream( ).setstate(state);

                try
                {
                    t.RestoreStream( );
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }

                SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                SUTL_TEST_ASSERT(StreamBufferIsZeroed(buf));
            }

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult GoodStream( )
        {
            TesterHelper t;
            std::wstringbuf buf;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                t.RestoreStream( );
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
            SUTL_TEST_ASSERT(StreamBufferIsZeroed(buf));

            SUTL_TEST_SUCCESS( );
        }
    }

    namespace Flush
    {
        /// Negative Tests \\\

        UnitTestResult NoBuffer( )
        {
            TesterHelper t;
            std::wstringbuf buf;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            t.GetStream( ).set_rdbuf(nullptr);

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
            SUTL_TEST_ASSERT(!t.GetStream( ).rdbuf( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) != &buf);
            SUTL_TEST_ASSERT(StreamBufferIsZeroed(buf));

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult BadStream( )
        {
            TesterHelper t;
            std::wstringbuf buf;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            for ( auto state : std::vector<int>{ std::ios_base::failbit, std::ios_base::badbit, std::ios_base::failbit | std::ios_base::badbit } )
            {
                t.GetStream( ).clear( );
                t.GetStream( ).setstate(state);

                try
                {
                    for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                    {
                        t.Flush(lvl);

                        SUTL_TEST_ASSERT(!t.GetStream( ).good( ));
                        SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                        SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                        SUTL_TEST_ASSERT(StreamBufferIsZeroed(buf));
                    }
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        /// Positive Tests \\\

        UnitTestResult NoContent( )
        {
            TesterHelper t;
            std::wstringbuf buf;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    t.Flush(lvl);

                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                    SUTL_TEST_ASSERT(StreamBufferIsZeroed(buf));
                }
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_SUCCESS( );
        }
    }

    namespace LogPrefixes
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult NoBuffer( )
        {
            TesterHelper t;
            std::wstringbuf buf;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            t.GetStream( ).set_rdbuf(nullptr);

            for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
            {
                bool threw = false;

                try
                {
                    t.LogPrefixes<T>(lvl, std::this_thread::get_id( ));
                }
                catch ( const std::exception& )
                {
                    threw = true;
                }

                SUTL_TEST_ASSERT(threw);
                SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                SUTL_TEST_ASSERT(!t.GetStream( ).rdbuf( ));
                SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) != &buf);
                SUTL_TEST_ASSERT(StreamBufferIsZeroed(buf));
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadStream( )
        {
            TesterHelper t;
            std::wstringbuf buf;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            for ( auto state : std::vector<int>{ std::ios_base::badbit, std::ios_base::failbit, std::ios_base::badbit | std::ios_base::failbit } )
            {
                t.GetStream( ).clear( );
                t.GetStream( ).setstate(state);

                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    bool threw = false;

                    try
                    {
                        t.LogPrefixes<T>(lvl, std::this_thread::get_id( ));
                    }
                    catch ( const std::exception& )
                    {
                        threw = true;
                    }

                    SUTL_TEST_ASSERT(threw);
                    SUTL_TEST_ASSERT(!t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                    SUTL_TEST_ASSERT(StreamBufferIsZeroed(buf));
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadVerbosityLevel( )
        {
            TesterHelper t;
            std::wstringbuf buf;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                t.LogPrefixes<T>(VerbosityLevel::MAX, std::this_thread::get_id( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
            SUTL_TEST_ASSERT(StreamBufferIsZeroed(buf));

            SUTL_TEST_SUCCESS( );
        }

        /// Positive Tests \\\

        template <class T>
        UnitTestResult GoodStream( )
        {
            TesterHelper t;
            std::wstringbuf buf;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(static_cast<SLL::OptionFlagType>(mask) + static_cast<SLL::OptionFlagType>(OptionFlag::PREFIX_BEGIN)) )
            {
                t.GetConfig( ).Disable(OptionFlag::PREFIX_MASK);
                t.GetConfig( ).Enable(mask);

                try
                {
                    t.LogPrefixes<T>(VerbosityLevel::INFO, std::this_thread::get_id( ));
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }

                SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                SUTL_TEST_ASSERT(!StreamBufferIsZeroed(buf));
                SUTL_TEST_ASSERT(StreamLoggerTests::ValidatePrefixes<wchar_t>(t.GetConfig( ), buf.str( )));

                STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
            }

            SUTL_TEST_SUCCESS( );
        }
    }

    namespace LogMessage
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult NoBuffer( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat(StringUtil::ConvertAndCopy<T>("Test string #%d"));
            bool threw = false;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            t.GetStream( ).set_rdbuf(nullptr);

            try
            {
                t.LogMessage<T>(pFormat.get( ), 1);
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_TEST_ASSERT(threw);
            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(!t.GetStream( ).rdbuf( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) != &buf);
            SUTL_TEST_ASSERT(StreamBufferIsZeroed(buf));

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadStream( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #%d.");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( auto state : std::vector<int>{ std::ios_base::badbit, std::ios_base::failbit, std::ios_base::badbit | std::ios_base::failbit } )
            {
                t.GetStream( ).clear( );
                t.GetStream( ).setstate(state);

                bool threw = false;

                try
                {
                    t.LogMessage<T>(pFormat.get( ), 1);
                }
                catch ( const std::exception& )
                {
                    threw = true;
                }

                SUTL_TEST_ASSERT(threw);
                SUTL_TEST_ASSERT(!t.GetStream( ).good( ));
                SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                SUTL_TEST_ASSERT(StreamBufferIsZeroed(buf));
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadFormat( )
        {
            TesterHelper t;
            std::wstringbuf buf;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            bool threw = false;

            try
            {
                t.LogMessage<T>(nullptr, 1);
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_TEST_ASSERT(threw);
            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
            SUTL_TEST_ASSERT(StreamBufferIsZeroed(buf));

            SUTL_TEST_SUCCESS( );
        }

        /// Positive Tests \\\
            
        template <class T>
        UnitTestResult ZeroLengthFormat( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

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

            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
            SUTL_TEST_ASSERT(CompareBufferWithString<char>(buf, "\n"));

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult PlainFormat( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string, no format specifiers.");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                t.LogMessage<T>(pFormat.get( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
            SUTL_TEST_ASSERT(CompareBufferWithString<char>(buf, "Test string, no format specifiers.\n"));

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult IntegralFormat( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string, integer specifiers %d %llu.");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                t.LogMessage<T>(pFormat.get( ), -500, 300000ull);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
            SUTL_TEST_ASSERT(CompareBufferWithString<char>(buf, "Test string, integer specifiers -500 300000.\n"));

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult FloatingFormat( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string, floating-point specifiers %2.2f %1.5f.");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                t.LogMessage<T>(pFormat.get( ), -35.019, 1.577789);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
            SUTL_TEST_ASSERT(CompareBufferWithString<char>(buf, "Test string, floating-point specifiers -35.02 1.57779.\n"));

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult StringFormat( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<T[ ]> arg1;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string, string specifiers %s %ls.");
                arg1 = StringUtil::ConvertAndCopy<T>("Test string #1");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                t.LogMessage<T>(pFormat.get( ), arg1.get( ), L"\"Test %f %g string %p #2\"");
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
            SUTL_TEST_ASSERT(CompareBufferWithString<char>(buf, "Test string, string specifiers Test string #1 \"Test %f %g string %p #2\".\n"));

            SUTL_TEST_SUCCESS( );
        }
    }

    namespace Log
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult NoBuffer( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #%d.");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string #1.\n");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(SLL::OptionFlagConverter::ToScalar(mask) + SLL::OptionFlagConverter::ToScalar(OptionFlag::PREFIX_BEGIN)) )
            {
                t.GetConfig( ).Disable(OptionFlag::PREFIX_MASK);
                t.GetConfig( ).Enable(mask);

                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    bool ret = false;
                    t.GetStream( ).set_rdbuf(nullptr);

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
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<wchar_t>(t.GetConfig( ), buf.str( ), pExpected));

                    STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadStream( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #%d.");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string #1.\n");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(SLL::OptionFlagConverter::ToScalar(mask) + SLL::OptionFlagConverter::ToScalar(OptionFlag::PREFIX_BEGIN)) )
            {
                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    for ( auto state : std::vector<int>{ std::ios_base::badbit, std::ios_base::failbit, std::ios_base::badbit | std::ios_base::failbit } )
                    {
                        bool ret = false;
                        t.GetStream( ).clear( );
                        t.GetStream( ).setstate(state);

                        try
                        {
                            ret = t.GetLogger( ).Log<T>(lvl, pFormat.get( ), 1);
                        }
                        catch ( const std::exception& e )
                        {
                            SUTL_SETUP_EXCEPTION(e.what( ));
                        }

                        SUTL_TEST_ASSERT(ret);
                        SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                        SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                        SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                        SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<wchar_t>(t.GetConfig( ), buf.str( ), pExpected));

                        STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
                    }
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadVerbosityLevel( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat;
            bool threw = false;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #d.");
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
            SUTL_TEST_ASSERT(t.GetStream( ).good( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
            SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
            SUTL_TEST_ASSERT(StreamBufferIsZeroed(buf));

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadFormat( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            bool threw = false;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(SLL::OptionFlagConverter::ToScalar(mask) + SLL::OptionFlagConverter::ToScalar(OptionFlag::PREFIX_BEGIN)) )
            {
                t.GetConfig( ).Disable(OptionFlag::PREFIX_MASK);
                t.GetConfig( ).Enable(mask);

                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    try
                    {
                        t.GetLogger( ).Log<T>(lvl, nullptr, 1);
                    }
                    catch ( const std::exception& )
                    {
                        threw = true;
                    }

                    SUTL_TEST_ASSERT(threw);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                    SUTL_TEST_ASSERT(StreamBufferIsZeroed(buf));
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        /// Positive Tests \\\

        template <class T>
        UnitTestResult ZeroLengthFormat( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("\n");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(SLL::OptionFlagConverter::ToScalar(mask) + SLL::OptionFlagConverter::ToScalar(OptionFlag::PREFIX_BEGIN)) )
            {
                t.GetConfig( ).Disable(OptionFlag::PREFIX_MASK);
                t.GetConfig( ).Enable(mask);

                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    bool ret = false;

                    try
                    {
                        ret = t.GetLogger( ).Log<T>(lvl, pFormat.get( ));
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<wchar_t>(t.GetConfig( ), buf.str( ), pExpected));

                    STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult PlainFormat( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string, no format specifiers.");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string, no format specifiers.\n");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(SLL::OptionFlagConverter::ToScalar(mask) + SLL::OptionFlagConverter::ToScalar(OptionFlag::PREFIX_BEGIN)) )
            {
                t.GetConfig( ).Disable(OptionFlag::PREFIX_MASK);
                t.GetConfig( ).Enable(mask);

                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    bool ret = false;

                    try
                    {
                        ret = t.GetLogger( ).Log<T>(lvl, pFormat.get( ));
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<wchar_t>(t.GetConfig( ), buf.str( ), pExpected));

                    STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult IntegralFormat( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string, integer specifiers %d %llu.");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string, integer specifiers -500 300000.\n");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(SLL::OptionFlagConverter::ToScalar(mask) + SLL::OptionFlagConverter::ToScalar(OptionFlag::PREFIX_BEGIN)) )
            {
                t.GetConfig( ).Disable(OptionFlag::PREFIX_MASK);
                t.GetConfig( ).Enable(mask);

                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    bool ret = false;

                    try
                    {
                        ret = t.GetLogger( ).Log<T>(lvl, pFormat.get( ), -500, 300000ull);
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<wchar_t>(t.GetConfig( ), buf.str( ), pExpected));

                    STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult FloatingFormat( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string, floating-point specifiers %2.2f %1.5f.");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string, floating-point specifiers -35.02 1.57779.\n");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(SLL::OptionFlagConverter::ToScalar(mask) + SLL::OptionFlagConverter::ToScalar(OptionFlag::PREFIX_BEGIN)) )
            {
                t.GetConfig( ).Disable(OptionFlag::PREFIX_MASK);
                t.GetConfig( ).Enable(mask);

                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    bool ret = false;

                    try
                    {
                        ret = t.GetLogger( ).Log<T>(lvl, pFormat.get( ), -35.019, 1.577789);
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<wchar_t>(t.GetConfig( ), buf.str( ), pExpected));

                    STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult StringFormat( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;
            std::unique_ptr<T[ ]> arg1;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string, string specifiers %s %ls.");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string, string specifiers Test string #1 \"Test %f %g string %p #2\".\n");
                arg1 = StringUtil::ConvertAndCopy<T>("Test string #1");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(SLL::OptionFlagConverter::ToScalar(mask) + SLL::OptionFlagConverter::ToScalar(OptionFlag::PREFIX_BEGIN)) )
            {
                t.GetConfig( ).Disable(OptionFlag::PREFIX_MASK);
                t.GetConfig( ).Enable(mask);

                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    bool ret = false;

                    try
                    {
                        ret = t.GetLogger( ).Log<T>(lvl, pFormat.get( ), arg1.get( ), L"\"Test %f %g string %p #2\"");
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<wchar_t>(t.GetConfig( ), buf.str( ), pExpected));

                    STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult RecoverFromNoBuffer( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string, no format specifiers.");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string, no format specifiers.\n");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(SLL::OptionFlagConverter::ToScalar(mask) + SLL::OptionFlagConverter::ToScalar(OptionFlag::PREFIX_BEGIN)) )
            {
                t.GetConfig( ).Disable(OptionFlag::PREFIX_MASK);
                t.GetConfig( ).Enable(mask);

                t.GetStream( ).set_rdbuf(nullptr);

                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    bool ret = false;

                    try
                    {
                        ret = t.GetLogger( ).Log<T>(lvl, pFormat.get( ));
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<wchar_t>(t.GetConfig( ), buf.str( ), pExpected));

                    STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult RecoverFromBadState( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string, no format specifiers.");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string, no format specifiers.\n");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(SLL::OptionFlagConverter::ToScalar(mask) + SLL::OptionFlagConverter::ToScalar(OptionFlag::PREFIX_BEGIN)) )
            {
                t.GetConfig( ).Disable(OptionFlag::PREFIX_MASK);
                t.GetConfig( ).Enable(mask);

                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    for ( auto state : std::vector<int>{ std::ios_base::badbit, std::ios_base::failbit, std::ios_base::badbit | std::ios_base::failbit } )
                    {
                        bool ret = false;
                        t.GetStream( ).clear( );
                        t.GetStream( ).setstate(state);

                        try
                        {
                            ret = t.GetLogger( ).Log<T>(lvl, pFormat.get( ));
                        }
                        catch ( const std::exception& e )
                        {
                            SUTL_TEST_EXCEPTION(e.what( ));
                        }

                        SUTL_TEST_ASSERT(ret);
                        SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                        SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                        SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                        SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<wchar_t>(t.GetConfig( ), buf.str( ), pExpected));

                        STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
                    }
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult NoLogBelowThreshold( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string, no format specifiers.");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string, no format specifiers.\n");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(SLL::OptionFlagConverter::ToScalar(mask) + SLL::OptionFlagConverter::ToScalar(OptionFlag::PREFIX_BEGIN)) )
            {
                t.GetConfig( ).Disable(OptionFlag::PREFIX_MASK);
                t.GetConfig( ).Enable(mask);

                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    for ( VerbosityLevel threshold = VerbosityLevel::BEGIN; threshold < VerbosityLevel::MAX; INCREMENT_VERBOSITY(threshold) )
                    {
                        bool ret = false;
                        t.GetConfig( ).SetVerbosityThreshold(threshold);

                        try
                        {
                            ret = t.GetLogger( ).Log<T>(lvl, pFormat.get( ));
                        }
                        catch ( const std::exception& e )
                        {
                            SUTL_TEST_EXCEPTION(e.what( ));
                        }

                        SUTL_TEST_ASSERT(ret);
                        SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                        SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                        SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);

                        if ( lvl < threshold )
                        {
                            SUTL_TEST_ASSERT(StreamBufferIsZeroed(buf));
                        }
                        else
                        {
                            SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<wchar_t>(t.GetConfig( ), buf.str( ), pExpected));
                        }

                        STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
                    }
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult FlushImportantMessage( )
        {
            TesterHelper t;
            StreamLoggerTests::TestStreamBuffer strbuf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, strbuf);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #%d.");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>("Test string #1.\n");
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

                    t.GetConfig( ).Enable(mask);

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
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &strbuf);

                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog(t.GetConfig( ), strbuf.str( ), pExpected));

                    if ( lvl >= VerbosityLevel::WARN )
                    {
                        SUTL_TEST_ASSERT(strbuf.Flushed( ));
                    }

                    strbuf = StreamLoggerTests::TestStreamBuffer( );
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult AppendStreamWrite( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected1;
            std::unique_ptr<wchar_t[ ]> pExpected2;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #%d.");
                pExpected1 = StringUtil::ConvertAndCopy<wchar_t>("Test string #1.\n");
                pExpected2 = StringUtil::ConvertAndCopy<wchar_t>("Test string #2.\n");
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
                    size_t pos = std::wstring::npos;

                    t.GetConfig( ).Enable(mask);

                    try
                    {
                        ret1 = t.GetLogger( ).Log<T>(lvl, pFormat.get( ), 1);
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret1);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);

                    try
                    {
                        ret2 = t.GetLogger( ).Log<T>(lvl, pFormat.get( ), 2);
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret2);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);

                    pos = buf.str( ).find(L'\n');

                    SUTL_TEST_ASSERT(pos != std::wstring::npos);
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog(t.GetConfig( ), buf.str( ).substr(0, pos + 1), pExpected1));
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog(t.GetConfig( ), buf.str( ).substr(pos + 1), pExpected2));

                    STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult ColorLog( )
        {
            TesterHelper t;
            std::wstringbuf buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<wchar_t[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            t.GetConfig( ).Enable(OptionFlag::LogInColor);

            try
            {
                pFormat = StringUtil::ConvertAndCopy<T>("Test string #%d.");
                pExpected = StringUtil::ConvertAndCopy<wchar_t>(("Test string #1." + t.GetColorSequences<char>( )[SLL::ColorConverter::ToScalar(Color::DEFAULT)] + "\n").c_str( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(static_cast<SLL::OptionFlagType>(mask) + static_cast<SLL::OptionFlagType>(OptionFlag::PREFIX_BEGIN)) )
            {
                t.GetConfig( ).Disable(OptionFlag::PREFIX_MASK);
                t.GetConfig( ).Enable(mask);

                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    for ( Color clr = Color::BASIC_BEGIN; clr < Color::MAX; clr = static_cast<Color>(SLL::ColorConverter::ToScalar(clr) + 1) )
                    {
                        const std::basic_string<wchar_t>& colorSequence = t.GetColorSequences<wchar_t>( )[SLL::ColorConverter::ToScalar(clr)];
                        bool ret = false;
                        size_t pos = std::wstring::npos;

                        t.GetConfig( ).SetColor(lvl, clr);

                        try
                        {
                            ret = t.GetLogger( ).Log<T>(lvl, pFormat.get( ), 1);
                        }
                        catch ( const std::exception& e )
                        {
                            SUTL_TEST_ASSERT(e.what( ));
                        }

                        SUTL_TEST_ASSERT(ret);

                        const std::wstring str = buf.str( );
                        SUTL_TEST_ASSERT(!str.empty( ));

                        pos = str.find(colorSequence);
                        SUTL_TEST_ASSERT(pos == 0);
                        SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog(t.GetConfig( ), str.substr(colorSequence.size( )), pExpected));
                        
                        STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
                    }
                }
            }

            SUTL_TEST_SUCCESS( );
        }
    }

}