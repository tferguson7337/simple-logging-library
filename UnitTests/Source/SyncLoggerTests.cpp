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
            Log::LogAsynchronously<char>,
            Log::LogAsynchronously<wchar_t>
        };

        return testList;
    }

    namespace Log
    {
        /// Log Test Helper Functions \\\

        std::wstring ReadFile(const std::wstring& f)
        {
            std::wstring str;
            std::wstring line;

            std::wifstream file(f);

            while ( std::getline(file, line) )
            {
                str.append(line);
            }

            return str;
        }

        bool AllMessagesInFile(const std::wstring& fileName)
        {
            // Substrings that we want to search for.
            static const wchar_t* numBegin = L"(#";
            static const wchar_t* numEnd = L")";

            unsigned long long msgMask = 0;
            size_t posBegin = 0;
            size_t posEnd = 0;

            // Read the file contents.
            std::wstring fileContents(ReadFile(fileName));
            while ( posBegin != std::wstring::npos && posEnd != std::wstring::npos )
            {
                // Find the first substr - "(#"
                posBegin = fileContents.find(numBegin, posEnd);
                if ( posBegin != std::wstring::npos )
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
                if ( posEnd == std::wstring::npos )
                {
                    // Couldn't find the end of the (#<num>) substring - exit.
                    break;
                }

                // Convert the number substring to unsigned long long representation.
                const unsigned long long n = _wcstoui64(&fileContents[posBegin], nullptr, 10);
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
            config.SetFile<wchar_t>(FileLoggerTests::GetGoodFileString( ));

            // Lambda function that the threads we kick off will execute.
            static const auto asyncLog = [&pLogger] (size_t num) -> bool
            {
                if constexpr ( std::is_same_v<T, char> )
                {
                    return pLogger->Log(VerbosityLevel::INFO, "Test log message (#%zu).", num);
                }
                else
                {
                    return pLogger->Log(VerbosityLevel::INFO, L"Test log message (#%zu).", num);
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