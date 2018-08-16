#pragma once

// Target Class
#include <FileLogger.h>

// LoggerBase - Helper Functions and Macros
#include <LoggerBaseTests.h>

// SUTL
#include <UnitTestResult.h>

// STL
#include <functional>
#include <list>
#include <tuple>

// C++17 - File-Deletion Cleanup
#include <filesystem>


namespace FileLoggerTests
{
    std::list<std::function<UnitTestResult(void)>> GetTests( );

    namespace InitializeFileStream
    {
        /// Negative Tests \\\

        UnitTestResult NoFile( );
        UnitTestResult BadPath( );

        /// Positive Tests \\\

        UnitTestResult GoodPath( );
    }

    namespace RestoreFileStream
    {
        /// Negative Tests \\\

        UnitTestResult NoFile( );
        UnitTestResult BadPath( );

        /// Positive Tests \\\

        UnitTestResult GoodPath( );
    }

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
        UnitTestResult RecoverFromNotOpen( );

        template <class T>
        UnitTestResult RecoverFromBadState( );

        template <class T>
        UnitTestResult NoLogBelowThreshold( );

        template <class T>
        UnitTestResult FlushImportantMessage( );

        template <class T>
        UnitTestResult AppendFileWrite( );
    }
}


namespace FileLoggerTests
{
    /// Helper Functions \\\

    // Build SLL::ConfigPackage object with specified filepath string and verbosity-threshold.
    inline SLL::ConfigPackage BuildConfig(const std::wstring& f, SLL::VerbosityLevel t)
    {
        SLL::ConfigPackage config;

        config.SetFile(f);
        config.SetVerbosityThreshold(t);

        return config;
    }

    // Return (hopefully) valid (relative) file path string.
    inline const std::wstring& GetGoodFileString( )
    {
        static const std::wstring str(std::filesystem::current_path( ).wstring( ) + L"\\" + L"test_file.log");

        return str;
    }

    // Return invalid file path string.
    inline const std::wstring& GetBadFileString( )
    {
        static const std::wstring str(L"\\\\BAD\\PATH\\NOT\\USABLE\\test_file.log");

        return str;
    }

    // Attempt to read test file contents and return them.
    inline std::wstring ReadTestFile( )
    {
        std::basic_ifstream<wchar_t> fileStream(GetGoodFileString( ), std::ios_base::in);
        std::wstring result;
        std::wstring line;

        while ( std::getline(fileStream, line) )
        {
            // If we retrieved content not ending with a newline, then append a newline.
            if ( !line.empty( ) && line.back( ) != L'\n' )
            {
                line.push_back(L'\n');
                result.append(line);
            }
        }

        // Trim off the last added newline character (if any).
        if ( !result.empty( ) && result.back( ) == L'\n' )
        {
            result.pop_back( );
        }

        return result;
    }

    // Attempt to delete the test file.
    inline bool DeleteTestFile( )
    {
        static const std::filesystem::path testFilePath(GetGoodFileString( ));

        // Delete file, double-check that it doesn't exist after deletion.
        return std::filesystem::remove(testFilePath) && !std::filesystem::exists(testFilePath);
    }

    inline size_t ValidatePrefixes(const SLL::ConfigPackage& config, const std::wstring& str)
    {
        typedef std::function<bool(const std::unique_ptr<wchar_t[ ]>&)> ValidityFunc;
        typedef std::tuple<SLL::OptionFlag, ValidityFunc, size_t> ValidityTuple;

        // Tuple = (OptionFlag, Validation Function, Expected Prefix String Size)
        static const std::vector<ValidityTuple> validityTuples
        {
            std::make_tuple(SLL::OptionFlag::LogTimestamp, LoggerBaseTests::IsTimePrefix<wchar_t>, 24),
            std::make_tuple(SLL::OptionFlag::LogThreadID, LoggerBaseTests::IsThreadIDPrefix<wchar_t>, 16),
            std::make_tuple(SLL::OptionFlag::LogVerbosityLevel, LoggerBaseTests::IsVerbosityLevelPrefix<wchar_t>, 14)
        };

        // Check to see if there's any work to do.
        if ( str.empty( ) || !config.OptionsEnabledAny(SLL::OptionFlag::PREFIX_MASK) )
        {
            return 0;
        }

        size_t i = 0;
        for ( const auto& tuple : validityTuples )
        {
            std::unique_ptr<wchar_t[ ]> prefixStr;

            // Extract tuple data.
            const SLL::OptionFlag&   flag = std::get<0>(tuple);
            const ValidityFunc& func = std::get<1>(tuple);
            const size_t&       size = std::get<2>(tuple);

            // See if we're testing for this flag.
            if ( !config.OptionEnabled(flag) )
            {
                continue;
            }

            // If we're going to go past string bounds, the test failed.
            if ( i + size > str.size( ) + 1 )
            {
                return 0;
            }

            // Get the target prefix substring.
            prefixStr = StringUtil::ConvertAndCopy<wchar_t>(str.substr(i, size - 1).c_str( ), size);

            // See if the prefix string is valid.
            if ( !func(prefixStr) )
            {
                return 0;
            }

            // Move string index to next prefix (or end).
            i += size - 1;
        }

        return i;
    }

    inline bool ValidateLog(const SLL::ConfigPackage& config, const std::wstring& str, const std::unique_ptr<wchar_t[ ]>& pExpected)
    {
        std::unique_ptr<wchar_t[ ]> logStr;
        size_t i = 0;

        // If str and pExpected are empty, then we're done.
        if ( str.empty( ) && (!pExpected || pExpected[0] == L'\0') )
        {
            return true;
        }

        // See if we're expecting any prefix content.
        if ( config.OptionsEnabledAny(SLL::OptionFlag::PREFIX_MASK) )
        {
            // Get index to start of main log message.
            i = ValidatePrefixes(config, str);

            // We're expecting at least one prefix - zero indicates failure.
            if ( i == 0 )
            {
                return false;
            }

            // We've reached string end after checking prefixes.
            if ( i == str.size( ) )
            {
                // If expected log is empty, we're good - otherwise return false.
                return (!pExpected || pExpected[0] == L'\0') ? true : false;
            }
        }

        // Extract log string.
        logStr = StringUtil::ConvertAndCopy<wchar_t>(str.substr(i).c_str( ), str.size( ) + 1 - i);

        return memcmp(logStr.get( ), pExpected.get( ), sizeof(wchar_t) * (str.size( ) + 1 - i)) == 0;
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

        std::unique_ptr<SLL::FileLogger> mpFileLogger;

        inline void CheckForNullLogger(const std::string& f) const
        {
            if ( !mpFileLogger )
            {
                throw std::logic_error(f + " - mpFileLogger is null.");
            }
        }

    public:
        /// Constructors \\\
        
        Tester( ) noexcept :
            mpFileLogger(nullptr)
        { }

        /// Destructor \\\

        ~Tester( ) = default;

        /// Getters \\\

        // File Logger Getter - const
        const SLL::FileLogger& GetLogger( ) const
        {
            CheckForNullLogger(__FUNCTION__);

            return *mpFileLogger;
        }

        // File Logger Getter - non-const
        SLL::FileLogger& GetLogger( )
        {
            CheckForNullLogger(__FUNCTION__);

            return *mpFileLogger;
        }

        // File Logger Stream Getter - const
        const std::basic_ofstream<wchar_t>& GetFileStream( ) const
        {
            CheckForNullLogger(__FUNCTION__);

            return mpFileLogger->mFileStream;
        }

        // File Logger Stream Getter - non-const
        std::basic_ofstream<wchar_t>& GetFileStream( )
        {
            CheckForNullLogger(__FUNCTION__);

            return mpFileLogger->mFileStream;
        }

        const SLL::ConfigPackage& GetConfig( ) const
        {
            CheckForNullLogger(__FUNCTION__);

            return mpFileLogger->GetConfig( );
        }

        SLL::ConfigPackage& GetConfig( )
        {
            CheckForNullLogger(__FUNCTION__);

            return mpFileLogger->GetConfig( );
        }

        /// Setters \\\

        // File Logger Setter - FileLogger [M]
        void SetFileLogger(SLL::FileLogger&& src)
        {
            mpFileLogger = std::make_unique<SLL::FileLogger>(std::move(src));
        }

        // File Logger Setter - ConfigPackage [C]
        void SetFileLogger(const SLL::ConfigPackage& config)
        {
            mpFileLogger = std::make_unique<SLL::FileLogger>(config);
        }

        // File Logger Setter - ConfigPackage [M]
        void SetFileLogger(SLL::ConfigPackage&& config)
        {
            mpFileLogger = std::make_unique<SLL::FileLogger>(std::move(config));
        }

        /// FileLogger Exposer Methods \\\

        void SetTargetFile(const std::wstring& f)
        {
            CheckForNullLogger(__FUNCTION__);

            mpFileLogger->GetConfig( ).SetFile(f);
        }

        void SetVerbosityThreshold(const SLL::VerbosityLevel& lvl)
        {
            CheckForNullLogger(__FUNCTION__);

            mpFileLogger->GetConfig( ).SetVerbosityThreshold(lvl);
        }

        // Initialize File Stream 
        void InitializeFileStream( )
        {
            CheckForNullLogger(__FUNCTION__);

            mpFileLogger->InitializeFileStream( );
        }

        // Restore File Stream.
        bool RestoreFileStream( )
        {
            CheckForNullLogger(__FUNCTION__);

            return mpFileLogger->RestoreFileStream( );
        }

        // Flush Content To File.
        void Flush(const SLL::VerbosityLevel& lvl)
        {
            CheckForNullLogger(__FUNCTION__);

            mpFileLogger->Flush(lvl);
        }

        // Log Prefixes to File.
        template <class T>
        void LogPrefixes(const SLL::VerbosityLevel& lvl, const std::thread::id& tid)
        {
            CheckForNullLogger(__FUNCTION__);

            mpFileLogger->LogPrefixes<T>(lvl, tid);
        }

        // Log User Message to File (...)
        template <class T>
        void LogMessage(const T* pFormat, ...)
        {
            va_list pArgs;

            CheckForNullLogger(__FUNCTION__);

            va_start(pArgs, pFormat);
            try
            {
                mpFileLogger->LogMessage(pFormat, pArgs);
            }
            catch ( ... )
            {
                va_end(pArgs);
                throw;
            }

            va_end(pArgs);
        }
    };
}