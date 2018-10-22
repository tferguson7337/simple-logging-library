#include <SyncLoggerTests.h>

#include <SyncLogger.h>

#include <future>

#include <FileLoggerTests.h>

namespace SyncLoggerTests
{
    using SLL::ConfigPackage;
    using SLL::OptionFlag;
    using SLL::VerbosityLevel;
    using SLL::SyncLogger;

    std::list<std::function<UnitTestResult(void)>> GetTests( )
    {
        static const std::list<std::function<UnitTestResult(void)>> testList
        {
            Log::LogAsynchronously<utf8>,
            Log::LogAsynchronously<utf16>
        };

        return testList;
    }

    namespace Log
    {
        /// Log Test Helper Functions \\\

        std::basic_string<utf16> ReadFile(const std::filesystem::path& f)
        {
            std::basic_string<utf16> str;
            std::basic_string<utf16> line;

            std::basic_ifstream<utf16> file(f);

            while ( std::getline(file, line) )
            {
                str.append(line);
            }

            return str;
        }

        bool AllMessagesInFile(const std::filesystem::path& fileName)
        {
            // Substrings that we want to search for.
            static const utf16* numBegin = UTF16_LITERAL_STR("(#");
            static const utf16* numEnd = UTF16_LITERAL_STR(")");

            unsigned long long msgMask = 0;
            size_t posBegin = 0;
            size_t posEnd = 0;

            // Read the file contents.
            std::basic_string<utf16> fileContents(ReadFile(fileName));
            while ( posBegin != std::basic_string<utf16>::npos && posEnd != std::basic_string<utf16>::npos )
            {
                // Find the first substr - "(#"
                posBegin = fileContents.find(numBegin, posEnd);
                if ( posBegin != std::basic_string<utf16>::npos )
                {
                    // Move to first character after '#'
                    posBegin += 2;
                }
                else
                {
                    // Couldn't find "(#" - exit.
                    break;
                }

                // Find the second substr - ")"
                posEnd = fileContents.find(numEnd, posBegin);
                if ( posEnd == std::basic_string<utf16>::npos )
                {
                    // Couldn't find the end of the (#<num>) substring - exit.
                    break;
                }

                // Convert the number substring to unsigned long long representation.
                const unsigned long long n = _wcstoui64(reinterpret_cast<const wchar_t*>(&fileContents[posBegin]), nullptr, 10);
                _ASSERTE(n < 64);

                // Flag the log message number in our mask.
                msgMask |= 1ull << ((n < 64) ? n : 0);
            }

            // If all 64 messages are in the file, all msgMask bits should be set.
            return msgMask == std::numeric_limits<unsigned long long>::max( );
        }

        template <class T>
        UnitTestResult LogAsynchronously( )
        {
            std::unique_ptr<SyncLogger> pLogger;
            std::vector<std::future<bool>> threads;

            // Setup the configuration package for SyncLogger.
            ConfigPackage config;
            config.Enable(OptionFlag::PREFIX_MASK | OptionFlag::LogToFile | OptionFlag::LogSynchronous | OptionFlag::LogInColor);
            config.SetFile(FileLoggerTests::GetGoodFilePath( ));

            // Lambda function that the threads we kick off will execute.
            static const auto asyncLog = [&pLogger] (size_t num) -> bool
            {
                if constexpr ( std::is_same_v<T, utf8> )
                {
                    return pLogger->Log(VerbosityLevel::INFO, "Test log message (#%zu).", num);
                }
                else if ( std::is_same_v<T, utf16> )
                {
                    return pLogger->Log(VerbosityLevel::INFO, UTF16_LITERAL_STR("Test log message (#%zu)."), num);
                }
                else
                {
                    throw std::runtime_error(__FUNCTION__": Invalid template argument type.");
                }
            };

            // Attempt to make the SyncLogger object.
            try
            {
                pLogger = std::make_unique<SyncLogger>(config);
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            // Attempt to kick off 64 threads that will submit numbered log messages to pLogger.
            try
            {
                for ( size_t i = 0; i < 64; i++ )
                {
                    threads.push_back(std::async(std::launch::async, asyncLog, i));
                }
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_ASSERT(e.what( ));
            }
            
            // Ensure that all threads we able to successfully submit their log messages.
            for ( auto& results : threads )
            {
                SUTL_TEST_ASSERT(results.get( ));
            }

            // Cleanup SyncLogger object.
            // This will wait on pLogger's worker thread to exit.
            pLogger.reset( );

            // Make sure all messages made it to the log file.
            SUTL_TEST_ASSERT(AllMessagesInFile(config.GetFile( )));

            // Attempt to cleanup test log file.
            SUTL_CLEANUP_ASSERT(FileLoggerTests::DeleteTestFile( ));

            SUTL_TEST_SUCCESS( );
        }
    }
}