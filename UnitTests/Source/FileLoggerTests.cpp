#include <FileLoggerTests.h>

#include <filesystem>

namespace FileLoggerTests
{
    std::list<std::function<UnitTestResult(void)>> GetTests( )
    {
        static const std::list<std::function<UnitTestResult(void)>> testList
        {
            /// Initialization Tests \\\

            InitializeFileStream,
            RestoreFileStream,

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
            Log::RecoverFromNotOpen<char>,
            Log::RecoverFromNotOpen<wchar_t>,

            Log::RecoverFromBadState<char>,
            Log::RecoverFromBadState<wchar_t>,

            Log::FlushImportantMessage<char>,
            Log::FlushImportantMessage<wchar_t>,

            Log::AppendFileWrite<char>,
            Log::AppendFileWrite<wchar_t>,
        };

        return testList;
    }

    /// Unit Tests \\\

    UnitTestResult InitializeFileStream( )
    {
        SUTL_SKIP_TEST("Implementation Pending...");
    }


    UnitTestResult RestoreFileStream( )
    {
        SUTL_SKIP_TEST("Implementation Pending...");
    }

    namespace Flush
    {
        /// Negative Tests \\\

        UnitTestResult NoFile( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }

        UnitTestResult BadStream( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }

        /// Positive Tests \\\

        UnitTestResult NoContent( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }

        UnitTestResult GoodStream( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }
    }

    namespace LogPrefixes
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult NoFile( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }

        template <class T>
        UnitTestResult BadStream( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }

        template <class T>
        UnitTestResult BadVerbosityLevel( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }

        /// Positive Tests \\\

        template <class T>
        UnitTestResult GoodStream( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }
    }

    namespace LogMessage
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult NoFile( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }


        template <class T>
        UnitTestResult BadStream( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }


        template <class T>
        UnitTestResult BadFormat( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }


        /// Positive Tests \\\
                
        template <class T>
        UnitTestResult ZeroLengthFormat( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }


        template <class T>
        UnitTestResult PlainFormat( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }


        template <class T>
        UnitTestResult IntegralFormat( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }


        template <class T>
        UnitTestResult FloatingFormat( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }


        template <class T>
        UnitTestResult StringFormat( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }
    }

    namespace Log
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult BadVerbosityLevel( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }


        template <class T>
        UnitTestResult BadFormat( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }


        /// Positive Tests \\\

        template <class T>
        UnitTestResult RecoverFromNotOpen( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }


        template <class T>
        UnitTestResult RecoverFromBadState( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }


        template <class T>
        UnitTestResult FlushImportantMessage( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }


        template <class T>
        UnitTestResult AppendFileWrite( )
        {
            SUTL_SKIP_TEST("Implementation Pending...");
        }
    }
}