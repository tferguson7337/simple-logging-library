#pragma once

// Target Class - StdOutLogger specialization.
#include <StreamLogger.h>

// LoggerBase - Helper Functions and Macros
#include <LoggerBaseTests.h>

// SUTL
#include <UnitTestResult.h>

namespace StdOutLoggerTests
{
    std::list<std::function<UnitTestResult(void)>> GetTests( );

    namespace InitializeStream
    {
        /// Negative Tests \\\

        UnitTestResult NoBuffer( );
        UnitTestResult BadStream( );

        /// Positive Tests \\\

        UnitTestResult GoodStream( );
    }

    namespace RestoreStream
    {
        /// Positive Tests \\\

        UnitTestResult NoBuffer( );
        UnitTestResult BadStream( );
        UnitTestResult GoodStream( );
    }

    namespace Flush
    {
        /// Negative Tests \\\

        UnitTestResult NoBuffer( );
        UnitTestResult BadStream( );

        /// Positive Tests \\\

        UnitTestResult NoContent( );
    }

    namespace LogPrefixes
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult NoBuffer( );

        template <class T>
        UnitTestResult BadStream( );

        template <class T>
        UnitTestResult BadVerbosityLevel( );

        /// Positive Tests \\\

        template <class T>
        UnitTestResult GoodStream( );
    }

    namespace LogMessage
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult NoBuffer( );

        template <class T>
        UnitTestResult BadStream( );

        template <class T>
        UnitTestResult BadFormat( );

        /// Positive Tests \\\
        
        template <class T>
        UnitTestResult ZeroLengthFormat( );

        template <class T>
        UnitTestResult PlainFormat( );

        template <class T>
        UnitTestResult IntegralFormat( );

        template <class T>
        UnitTestResult FloatingFormat( );

        template <class T>
        UnitTestResult StringFormat( );
    }

    namespace Log
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult NoBuffer( );

        template <class T>
        UnitTestResult BadStream( );

        template <class T>
        UnitTestResult BadVerbosityLevel( );

        template <class T>
        UnitTestResult BadFormat( );

        /// Positive Tests \\\

        template <class T>
        UnitTestResult ZeroLengthFormat( );

        template <class T>
        UnitTestResult PlainFormat( );

        template <class T>
        UnitTestResult IntegralFormat( );

        template <class T>
        UnitTestResult FloatingFormat( );

        template <class T>
        UnitTestResult StringFormat( );

        template <class T>
        UnitTestResult RecoverFromNoBuffer( );

        template <class T>
        UnitTestResult RecoverFromBadState( );

        template <class T>
        UnitTestResult NoLogBelowThreshold( );

        template <class T>
        UnitTestResult FlushImportantMessage( );

        template <class T>
        UnitTestResult AppendStreamWrite( );

        template <class T>
        UnitTestResult ColorLog( );
    }
}

namespace StreamLoggerTests
{
    class TestStreamBuffer : public std::basic_stringbuf<utf16>
    {
        // Follow basic_stringbuf - deleted copy.
        TestStreamBuffer(const TestStreamBuffer&) = delete;
        TestStreamBuffer& operator=(const TestStreamBuffer&) = delete;

    private:
        // Track whether the stream associated with this buffer was flushed.
        bool mFlushed;

    public:
        /// Constructors \\\

        // Default Constructor.
        TestStreamBuffer( ) :
            mFlushed(false),
            std::basic_stringbuf<utf16>( )
        { }

        // Move Constructor.
        TestStreamBuffer(TestStreamBuffer&& src) :
            mFlushed(false),
            std::basic_stringbuf<utf16>(std::move(src))
        { 
            mFlushed = src.mFlushed;
        }

        /// Destructor \\\

        // Virtual Destructor, default.
        virtual ~TestStreamBuffer( ) = default;

        /// Assignment Overloads \\\

        // Move Assignment.
        TestStreamBuffer& operator=(TestStreamBuffer&& src)
        {
            std::basic_stringbuf<utf16>::operator=(std::move(src));
            mFlushed = src.mFlushed;

            return *this;
        }

        /// wstringbuf Wrapper \\\

        // Detect call to sync when the stream is flushed.
        virtual int sync( )
        {
            int ret = std::basic_stringbuf<utf16>::sync( );
            if ( ret != -1 )
            {
                mFlushed = true;
            }

            return ret;
        }

        /// Getter/Setter \\\

        // Return whether or not the stream has been flushed since last call to this method.
        bool Flushed( )
        {
            bool old = mFlushed;
            mFlushed = false;
            return old;
        }
    };
}

namespace StdOutLoggerTests
{
    // Build SLL::ConfigPackage object with specified filepath string and verbosity-threshold.
    inline SLL::ConfigPackage BuildConfig(SLL::VerbosityLevel t)
    {
        SLL::ConfigPackage config;

        config.SetVerbosityThreshold(t);

        return config;
    }
}
