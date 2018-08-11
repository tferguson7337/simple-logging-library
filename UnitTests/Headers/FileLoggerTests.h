#pragma once

// Target Class
#include <FileLogger.h>

// SUTL
#include <UnitTestResult.h>

// STL
#include <functional>
#include <list>


namespace FileLoggerTests
{
    std::list<std::function<UnitTestResult(void)>> GetTests( );

    UnitTestResult InitializeFileStream( );
    UnitTestResult RestoreFileStream( );

    namespace Flush
    {
        /// Negative Tests \\\

        UnitTestResult NoFile( );
        UnitTestResult BadStream( );

        /// Positive Tests \\\

        UnitTestResult NoContent( );
        UnitTestResult GoodStream( );
    }
    
    
    namespace LogPrefixes
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult NoFile( );

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
        UnitTestResult NoFile( );

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
        UnitTestResult BadVerbosityLevel( );

        template <class T>
        UnitTestResult BadFormat( );

        /// Positive Tests \\\

        template <class T>
        UnitTestResult RecoverFromNotOpen( );

        template <class T>
        UnitTestResult RecoverFromBadState( );

        template <class T>
        UnitTestResult FlushImportantMessage( );

        template <class T>
        UnitTestResult AppendFileWrite( );
    }
}

namespace FileLoggerTests
{
    class Tester
    {
        /// No copy or move allowed.
        Tester(const Tester&) = delete;
        Tester(Tester&&) = delete;

        Tester& operator=(const Tester&) = delete;
        Tester& operator=(Tester&&) = delete;

    private:
        /// Private Data Member \\\

        SLL::FileLogger mFileLogger;

    public:
        /// Constructors \\\
        
        Tester(const SLL::ConfigPackage& src) :
            mFileLogger(src)
        { }

        Tester(SLL::ConfigPackage&& src) :
            mFileLogger(std::move(src))
        { }


        /// Destructor \\\

        ~Tester( ) = default;


        /// Getters \\\

        // File Logger Getter - const
        const SLL::FileLogger& GetLogger( ) const noexcept
        {
            return mFileLogger;
        }

        // File Logger Getter - non-const
        SLL::FileLogger& GetLogger( ) noexcept
        {
            return mFileLogger;
        }

        // File Logger Stream Getter - const
        const std::basic_ofstream<wchar_t>& GetFileStream( ) const noexcept
        {
            return mFileLogger.mFileStream;
        }

        // File Logger Stream Getter - non-const
        std::basic_ofstream<wchar_t>& GetFileStream( ) noexcept
        {
            return mFileLogger.mFileStream;
        }
    };
}