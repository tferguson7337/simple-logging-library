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

	using ReturnType = CC::StringUtil::ReturnType;

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

            LogPrefixes::NoBuffer<utf8>,
            LogPrefixes::NoBuffer<utf16>,

            LogPrefixes::BadStream<utf8>,
            LogPrefixes::BadStream<utf16>,

            LogPrefixes::BadVerbosityLevel<utf8>,
            LogPrefixes::BadVerbosityLevel<utf16>,

            /// Positive Tests \\\

            LogPrefixes::GoodStream<utf8>,
            LogPrefixes::GoodStream<utf16>,

            // LogMessage Tests

            /// Negative Tests \\\

            LogMessage::NoBuffer<utf8>,
            LogMessage::NoBuffer<utf16>,

            LogMessage::BadStream<utf8>,
            LogMessage::BadStream<utf16>,

            LogMessage::BadFormat<utf8>,
            LogMessage::BadFormat<utf16>,

            /// Positive Tests \\\
                                                
            LogMessage::ZeroLengthFormat<utf8>,
            LogMessage::ZeroLengthFormat<utf16>,

            LogMessage::PlainFormat<utf8>,
            LogMessage::PlainFormat<utf16>,

            LogMessage::IntegralFormat<utf8>,
            LogMessage::IntegralFormat<utf16>,

            LogMessage::FloatingFormat<utf8>,
            LogMessage::FloatingFormat<utf16>,

            LogMessage::StringFormat<utf8>,
            LogMessage::StringFormat<utf16>,

            // Log Tests

            /// Negative Tests \\\

            Log::NoBuffer<utf8>,
            Log::NoBuffer<utf16>,

            Log::BadStream<utf8>,
            Log::BadStream<utf16>,

            Log::BadVerbosityLevel<utf8>,
            Log::BadVerbosityLevel<utf16>,

            Log::BadFormat<utf8>,
            Log::BadFormat<utf16>,

            /// Positive Tests \\\

            Log::ZeroLengthFormat<utf8>,
            Log::ZeroLengthFormat<utf16>,

            Log::PlainFormat<utf8>,
            Log::PlainFormat<utf16>,

            Log::IntegralFormat<utf8>,
            Log::IntegralFormat<utf16>,

            Log::FloatingFormat<utf8>,
            Log::FloatingFormat<utf16>,

            Log::StringFormat<utf8>,
            Log::StringFormat<utf16>,

            Log::RecoverFromNoBuffer<utf8>,
            Log::RecoverFromNoBuffer<utf16>,

            Log::RecoverFromBadState<utf8>,
            Log::RecoverFromBadState<utf16>,

            Log::NoLogBelowThreshold<utf8>,
            Log::NoLogBelowThreshold<utf16>,

            Log::FlushImportantMessage<utf8>,
            Log::FlushImportantMessage<utf16>,

            Log::AppendStreamWrite<utf8>,
            Log::AppendStreamWrite<utf16>,

            Log::ColorLog<utf8>,
            Log::ColorLog<utf16>,
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
#define STDOUT_LOGGER_TEST_RESET_BUFFER(buf)            \
    buf = std::basic_stringbuf<utf16>( )                \


    // Return true if the buffer is completely filled with zero values, false otherwise.
    bool StreamBufferIsZeroed(const std::basic_stringbuf<utf16>& buf)
    {
        for ( auto c : buf.str( ) )
        {
            if ( c != static_cast<utf16>('\0') )
            {
                return false;
            }
        }

        return true;
    }

    template <class T>
    bool CompareBufferWithString(const std::basic_stringbuf<utf16>& buf, const std::basic_string<T>& str)
    {
        const std::basic_string<T> bufferString = CC::StringUtil::UTFConversion<ReturnType::StringObj, T>(buf.str( ));

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
            std::basic_stringbuf<utf16> buf;

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
            std::basic_stringbuf<utf16> buf;
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
            std::basic_stringbuf<utf16> buf;

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
            std::basic_stringbuf<utf16> buf;

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
            std::basic_stringbuf<utf16> buf;

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
            std::basic_stringbuf<utf16> buf;

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
            std::basic_stringbuf<utf16> buf;

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
            std::basic_stringbuf<utf16> buf;

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
            std::basic_stringbuf<utf16> buf;

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
            std::basic_stringbuf<utf16> buf;

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
            std::basic_stringbuf<utf16> buf;

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
            std::basic_stringbuf<utf16> buf;

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
            std::basic_stringbuf<utf16> buf;

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
                SUTL_TEST_ASSERT(StreamLoggerTests::ValidatePrefixes<utf16>(t.GetConfig( ), buf.str( )));

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
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat(CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string #%d"));
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
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string #%d.");
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
            std::basic_stringbuf<utf16> buf;

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
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("");
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
            SUTL_TEST_ASSERT(CompareBufferWithString<utf8>(buf, "\n"));

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult PlainFormat( )
        {
            TesterHelper t;
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string, no format specifiers.");
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
            SUTL_TEST_ASSERT(CompareBufferWithString<utf8>(buf, "Test string, no format specifiers.\n"));

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult IntegralFormat( )
        {
            TesterHelper t;
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string, integer specifiers %d %llu.");
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
            SUTL_TEST_ASSERT(CompareBufferWithString<utf8>(buf, "Test string, integer specifiers -500 300000.\n"));

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult FloatingFormat( )
        {
            TesterHelper t;
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string, floating-point specifiers %2.2f %1.5f.");
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
            SUTL_TEST_ASSERT(CompareBufferWithString<utf8>(buf, "Test string, floating-point specifiers -35.02 1.57779.\n"));

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult StringFormat( )
        {
            TesterHelper t;
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<T[ ]> arg1;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string, string specifiers %s %ls.");
                arg1 = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string #1");
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
            SUTL_TEST_ASSERT(CompareBufferWithString<utf8>(buf, "Test string, string specifiers Test string #1 \"Test %f %g string %p #2\".\n"));

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
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<utf16[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string #%d.");
                pExpected = CC::StringUtil::UTFConversion<ReturnType::SmartCString, utf16>("Test string #1.\n");
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
                        ret = t.GetLogger( ).Log(lvl, pFormat.get( ), 1);
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<utf16>(t.GetConfig( ), buf.str( ), pExpected));

                    STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult BadStream( )
        {
            TesterHelper t;
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<utf16[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string #%d.");
                pExpected = CC::StringUtil::UTFConversion<ReturnType::SmartCString, utf16>("Test string #1.\n");
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
                            ret = t.GetLogger( ).Log(lvl, pFormat.get( ), 1);
                        }
                        catch ( const std::exception& e )
                        {
                            SUTL_SETUP_EXCEPTION(e.what( ));
                        }

                        SUTL_TEST_ASSERT(ret);
                        SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                        SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                        SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                        SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<utf16>(t.GetConfig( ), buf.str( ), pExpected));

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
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat;
            bool threw = false;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string #d.");
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                t.GetLogger( ).Log(VerbosityLevel::MAX, pFormat.get( ), 1);
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
            std::basic_stringbuf<utf16> buf;
            bool threw = false;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            for ( OptionFlag mask = OptionFlag::PREFIX_BEGIN; mask < OptionFlag::PREFIX_END; mask = static_cast<OptionFlag>(SLL::OptionFlagConverter::ToScalar(mask) + SLL::OptionFlagConverter::ToScalar(OptionFlag::PREFIX_BEGIN)) )
            {
                t.GetConfig( ).Disable(OptionFlag::PREFIX_MASK);
                t.GetConfig( ).Enable(mask);

                for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
                {
                    const T* nullFormat = nullptr;

                    try
                    {
                        t.GetLogger( ).Log(lvl, nullFormat, 1);
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
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<utf16[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("");
                pExpected = CC::StringUtil::UTFConversion<ReturnType::SmartCString, utf16>("\n");
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
                        ret = t.GetLogger( ).Log(lvl, pFormat.get( ));
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<utf16>(t.GetConfig( ), buf.str( ), pExpected));

                    STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult PlainFormat( )
        {
            TesterHelper t;
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<utf16[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string, no format specifiers.");
                pExpected = CC::StringUtil::UTFConversion<ReturnType::SmartCString, utf16>("Test string, no format specifiers.\n");
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
                        ret = t.GetLogger( ).Log(lvl, pFormat.get( ));
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<utf16>(t.GetConfig( ), buf.str( ), pExpected));

                    STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult IntegralFormat( )
        {
            TesterHelper t;
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<utf16[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string, integer specifiers %d %llu.");
                pExpected = CC::StringUtil::UTFConversion<ReturnType::SmartCString, utf16>("Test string, integer specifiers -500 300000.\n");
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
                        ret = t.GetLogger( ).Log(lvl, pFormat.get( ), -500, 300000ull);
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<utf16>(t.GetConfig( ), buf.str( ), pExpected));

                    STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult FloatingFormat( )
        {
            TesterHelper t;
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<utf16[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string, floating-point specifiers %2.2f %1.5f.");
                pExpected = CC::StringUtil::UTFConversion<ReturnType::SmartCString, utf16>("Test string, floating-point specifiers -35.02 1.57779.\n");
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
                        ret = t.GetLogger( ).Log(lvl, pFormat.get( ), -35.019, 1.577789);
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<utf16>(t.GetConfig( ), buf.str( ), pExpected));

                    STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult StringFormat( )
        {
            TesterHelper t;
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<utf16[ ]> pExpected;
            std::unique_ptr<T[ ]> arg1;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string, string specifiers %s %ls.");
                pExpected = CC::StringUtil::UTFConversion<ReturnType::SmartCString, utf16>("Test string, string specifiers Test string #1 \"Test %f %g string %p #2\".\n");
                arg1 = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string #1");
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
                        ret = t.GetLogger( ).Log(lvl, pFormat.get( ), arg1.get( ), L"\"Test %f %g string %p #2\"");
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<utf16>(t.GetConfig( ), buf.str( ), pExpected));

                    STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult RecoverFromNoBuffer( )
        {
            TesterHelper t;
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<utf16[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string, no format specifiers.");
                pExpected = CC::StringUtil::UTFConversion<ReturnType::SmartCString, utf16>("Test string, no format specifiers.\n");
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
                        ret = t.GetLogger( ).Log(lvl, pFormat.get( ));
                    }
                    catch ( const std::exception& e )
                    {
                        SUTL_TEST_EXCEPTION(e.what( ));
                    }

                    SUTL_TEST_ASSERT(ret);
                    SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                    SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                    SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<utf16>(t.GetConfig( ), buf.str( ), pExpected));

                    STDOUT_LOGGER_TEST_RESET_BUFFER(buf);
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        template <class T>
        UnitTestResult RecoverFromBadState( )
        {
            TesterHelper t;
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<utf16[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string, no format specifiers.");
                pExpected = CC::StringUtil::UTFConversion<ReturnType::SmartCString, utf16>("Test string, no format specifiers.\n");
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
                            ret = t.GetLogger( ).Log(lvl, pFormat.get( ));
                        }
                        catch ( const std::exception& e )
                        {
                            SUTL_TEST_EXCEPTION(e.what( ));
                        }

                        SUTL_TEST_ASSERT(ret);
                        SUTL_TEST_ASSERT(t.GetStream( ).good( ));
                        SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ));
                        SUTL_TEST_ASSERT(t.GetStream( ).rdbuf( ) == &buf);
                        SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<utf16>(t.GetConfig( ), buf.str( ), pExpected));

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
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<utf16[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string, no format specifiers.");
                pExpected = CC::StringUtil::UTFConversion<ReturnType::SmartCString, utf16>("Test string, no format specifiers.\n");
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
                            ret = t.GetLogger( ).Log(lvl, pFormat.get( ));
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
                            SUTL_TEST_ASSERT(StreamLoggerTests::ValidateLog<utf16>(t.GetConfig( ), buf.str( ), pExpected));
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
            std::unique_ptr<utf16[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, strbuf);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string #%d.");
                pExpected = CC::StringUtil::UTFConversion<ReturnType::SmartCString, utf16>("Test string #1.\n");
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
                        ret = t.GetLogger( ).Log(lvl, pFormat.get( ), 1);
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
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<utf16[ ]> pExpected1;
            std::unique_ptr<utf16[ ]> pExpected2;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string #%d.");
                pExpected1 = CC::StringUtil::UTFConversion<ReturnType::SmartCString, utf16>("Test string #1.\n");
                pExpected2 = CC::StringUtil::UTFConversion<ReturnType::SmartCString, utf16>("Test string #2.\n");
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
                    size_t pos = std::basic_string<utf16>::npos;

                    t.GetConfig( ).Enable(mask);

                    try
                    {
                        ret1 = t.GetLogger( ).Log(lvl, pFormat.get( ), 1);
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
                        ret2 = t.GetLogger( ).Log(lvl, pFormat.get( ), 2);
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

                    SUTL_TEST_ASSERT(pos != std::basic_string<utf16>::npos);
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
            std::basic_stringbuf<utf16> buf;
            std::unique_ptr<T[ ]> pFormat;
            std::unique_ptr<utf16[ ]> pExpected;

            STDOUT_LOGGER_TEST_COMMON_SETUP(t, buf);

            t.GetConfig( ).Enable(OptionFlag::LogInColor);

            try
            {
                pFormat = CC::StringUtil::UTFConversion<ReturnType::SmartCString, T>("Test string #%d.");
                pExpected = CC::StringUtil::UTFConversion<ReturnType::SmartCString, utf16>(("Test string #1." + std::get<std::basic_string<utf8>>(t.GetColorSequences( )[SLL::ColorConverter::ToScalar(Color::DEFAULT)]) + "\n").c_str( ));
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
                        const std::basic_string<utf16>& colorSequence = std::get<std::basic_string<utf16>>(t.GetColorSequences( )[SLL::ColorConverter::ToScalar(clr)]);
                        bool ret = false;
                        size_t pos = std::basic_string<utf16>::npos;

                        t.GetConfig( ).SetColor(lvl, clr);

                        try
                        {
                            ret = t.GetLogger( ).Log(lvl, pFormat.get( ), 1);
                        }
                        catch ( const std::exception& e )
                        {
                            SUTL_TEST_ASSERT(e.what( ));
                        }

                        SUTL_TEST_ASSERT(ret);

                        const std::basic_string<utf16> str = buf.str( );
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