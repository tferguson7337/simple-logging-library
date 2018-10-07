#pragma once

// Target Class - FileLogger specialization.
#include <StreamLogger.h>

// LoggerBase - Helper Functions and Macros
#include <LoggerBaseTests.h>

// SUTL
#include <UnitTestResult.h>

// STL - Tuples for Validation Helpers
#include <tuple>

// C++17 - File-Deletion Cleanup
#include <filesystem>


namespace FileLoggerTests
{
    std::list<std::function<UnitTestResult(void)>> GetTests( );

    namespace InitializeStream
    {
        /// Negative Tests \\\

        UnitTestResult NoFile( );
        UnitTestResult BadPath( );

        /// Positive Tests \\\

        UnitTestResult GoodPath( );
    }

    namespace RestoreStream
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
        UnitTestResult NoFile( );

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
    inline SLL::ConfigPackage BuildConfig(const std::filesystem::path& p, SLL::VerbosityLevel t)
    {
        SLL::ConfigPackage config;

        config.SetFile(p);
        config.SetVerbosityThreshold(t);

        return config;
    }

    // Return (hopefully) valid (relative) file path string.
    inline const std::filesystem::path& GetGoodFilePath( )
    {
        static const std::filesystem::path goodPath(std::filesystem::current_path( ).u16string( ) + u"\\" + u"test_file.log");

        return goodPath;
    }

    inline const std::filesystem::path& GetBadFilePath( )
    {
        static const std::filesystem::path badPath(u"\\\\BADPATH\\PATH\\IS\\NO\\GOOD");

        return badPath;
    }

    // Attempt to read test file contents and return them.
    inline std::basic_string<utf16> ReadTestFile( )
    {
        std::basic_ifstream<utf16> fileStream(GetGoodFilePath( ), std::ios_base::in);
        std::basic_string<utf16> result;
        std::basic_string<utf16> line;

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
        static const std::filesystem::path testFilePath(GetGoodFilePath( ));

        // Delete file, double-check that it doesn't exist after deletion.
        return std::filesystem::remove(testFilePath) && !std::filesystem::exists(testFilePath);
    }
}

namespace StreamLoggerTests
{
    /// Helper Functions \\\

    template <class T>
    inline size_t ValidatePrefixes(const SLL::ConfigPackage& config, const std::basic_string<T>& str)
    {
        typedef std::function<bool(const std::unique_ptr<T[ ]>&)> ValidityFunc;
        typedef std::tuple<SLL::OptionFlag, ValidityFunc, size_t> ValidityTuple;

        // Tuple = (OptionFlag, Validation Function, Expected Prefix String Size)
        static const std::vector<ValidityTuple> validityTuples
        {
            std::make_tuple(SLL::OptionFlag::LogTimestamp, LoggerBaseTests::IsTimePrefix<T>, 24),
            std::make_tuple(SLL::OptionFlag::LogThreadID, LoggerBaseTests::IsThreadIDPrefix<T>, 16),
            std::make_tuple(SLL::OptionFlag::LogVerbosityLevel, LoggerBaseTests::IsVerbosityLevelPrefix<T>, 14)
        };

        // Check to see if there's any work to do.
        if ( str.empty( ) || !config.OptionsEnabledAny(SLL::OptionFlag::PREFIX_MASK) )
        {
            return 0;
        }

        size_t i = 0;
        for ( const auto& tuple : validityTuples )
        {
            std::unique_ptr<T[ ]> prefixStr;

            // Extract tuple data.
            const SLL::OptionFlag&  flag = std::get<0>(tuple);
            const ValidityFunc&     func = std::get<1>(tuple);
            const size_t&           size = std::get<2>(tuple);

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
            prefixStr = StringUtil::ConvertAndCopy<T>(str.substr(i, size - 1).c_str( ), size);

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

    template <class T>
    inline bool ValidateLog(const SLL::ConfigPackage& config, const std::basic_string<T>& str, const std::unique_ptr<T[ ]>& pExpected)
    {
        std::unique_ptr<T[ ]> logStr;
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
        logStr = StringUtil::ConvertAndCopy<T>(str.substr(i).c_str( ), str.size( ) + 1 - i);

        return memcmp(logStr.get( ), pExpected.get( ), sizeof(T) * (str.size( ) + 1 - i)) == 0;
    }


    template <class LoggerType, class StreamType>
    class Tester
    {
        /// No copy or move allowed.
        Tester(const Tester&) = delete;
        Tester(Tester&&) = delete;

        Tester& operator=(const Tester&) = delete;
        Tester& operator=(Tester&&) = delete;

    private:
        /// Private Data Member \\\

        std::unique_ptr<LoggerType> mpLogger;

        inline void CheckForNullLogger(const std::string& f) const
        {
            if ( !mpLogger )
            {
                throw std::logic_error(f + " - mpLogger is null.");
            }
        }

    public:
        /// Constructors \\\
        
        Tester( ) noexcept :
            mpLogger(nullptr)
        { }

        /// Destructor \\\

        ~Tester( ) = default;

        /// Getters \\\

        // Logger Getter - const
        const LoggerType& GetLogger( ) const
        {
            CheckForNullLogger(__FUNCTION__);

            return *mpLogger;
        }

        // Logger Getter - non-const
        LoggerType& GetLogger( )
        {
            CheckForNullLogger(__FUNCTION__);

            return *mpLogger;
        }

        // Logger Stream Getter - const
        const StreamType& GetStream( ) const
        {
            CheckForNullLogger(__FUNCTION__);

            return mpLogger->mStream;
        }

        // Logger Stream Getter - non-const
        StreamType& GetStream( )
        {
            CheckForNullLogger(__FUNCTION__);

            return mpLogger->mStream;
        }

        // Logger ConfigPackage Getter - const
        const SLL::ConfigPackage& GetConfig( ) const
        {
            CheckForNullLogger(__FUNCTION__);

            return mpLogger->GetConfig( );
        }

        // Logger ConfigPackage Getter - non-const
        SLL::ConfigPackage& GetConfig( )
        {
            CheckForNullLogger(__FUNCTION__);

            return mpLogger->GetConfig( );
        }

        // Logger Color Sequence Getter - const
        const std::vector<SupportedStringTuple>& GetColorSequences( ) const
        {
            CheckForNullLogger(__FUNCTION__);

            return mpLogger->mColorSequences;
        }

        /// Setters \\\

        // Logger Setter - Logger [M]
        void SetLogger(LoggerType&& src)
        {
            mpLogger = std::make_unique<LoggerType>(std::move(src));
        }

        // Logger Setter - ConfigPackage [C]
        void SetLogger(const SLL::ConfigPackage& config)
        {
            mpLogger = std::make_unique<LoggerType>(config);
        }

        // Logger Setter - ConfigPackage [M]
        void SetLogger(SLL::ConfigPackage&& config)
        {
            mpLogger = std::make_unique<LoggerType>(std::move(config));
        }

        void SetStreamBuffer(std::basic_streambuf<utf16>& buf)
        {
            CheckForNullLogger(__FUNCTION__);

            mpLogger->mpUTF16StreamBuffer = &buf;
        }

        /// StreamLogger Exposer Methods \\\

        // Initialize Stream 
        void InitializeStream( )
        {
            CheckForNullLogger(__FUNCTION__);

            mpLogger->InitializeStream( );
        }

        // Restore Stream.
        bool RestoreStream( )
        {
            CheckForNullLogger(__FUNCTION__);

            return mpLogger->RestoreStream( );
        }

        // Flush Content To File.
        void Flush(const SLL::VerbosityLevel& lvl)
        {
            CheckForNullLogger(__FUNCTION__);

            mpLogger->Flush(lvl);
        }

        // Log Prefixes to File.
        template <class T>
        void LogPrefixes(const SLL::VerbosityLevel& lvl, const std::thread::id& tid)
        {
            CheckForNullLogger(__FUNCTION__);

            mpLogger->LogPrefixes<T>(lvl, tid);
        }

        // Log User Message to File (...)
        template <class T>
        void LogMessage(const T* pFormat, ...)
        {
            va_list pArgs;

            CheckForNullLogger(__FUNCTION__);

            try
            {
                va_start(pArgs, pFormat);
                mpLogger->LogMessage<T>(pFormat, pArgs);
            }
            catch ( const std::exception& )
            {
                va_end(pArgs);
                throw;
            }

            va_end(pArgs);
        }
    };
}