// Test Function Definitions
#include <DualLoggerTests.h>

// SLL
#include <ConfigPackage.h>
#include <DualLogger.h>

// Helper Utilities for Testing
#include <LoggerBaseTests.h>

// C++17 Filesystem
#include <filesystem>

namespace DualLoggerTests
{
    using SLL::ConfigPackage;
    using SLL::DualLogger;

    std::list<std::function<UnitTestResult(void)>> GetTests( )
    {
        static const std::list<std::function<UnitTestResult(void)>> testList
        {
            // SharedConfig Tests
    
            /// Negative Tests \\\

            SharedConfig::NoFile,
            SharedConfig::BadPath,

            /// Positive Tests \\\

            SharedConfig::GoodPath,
        

            // SeparateConfig Tests
        
            /// Negative Tests \\\

            SeparateConfig::FileLoggerNoFile,
            SeparateConfig::FileLoggerBadPath,

            /// Positive Tests \\\

            SeparateConfig::StdOutLoggerNoFile,
            SeparateConfig::StdOutLoggerBadPath,
        

            // GetConfig Tests
        
            /// Positive Tests \\\

            GetConfig::GetFileLoggerConfig,
            GetConfig::GetStdOutLoggerConfig,
        
        };

        return testList;
    }


    namespace SharedConfig
    {
        /// Negative Tests \\\

        UnitTestResult NoFile( )
        {
            ConfigPackage sharedConfig;
            std::unique_ptr<DualLogger> pDualLogger;
            bool threw = false;

            try
            {
                pDualLogger = std::make_unique<DualLogger>(sharedConfig);
            }
            catch ( const std::bad_alloc& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }
            catch ( const std::logic_error& )
            {
                threw = true;
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(threw);

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult BadPath( )
        {
            ConfigPackage sharedConfig;
            std::unique_ptr<DualLogger> pDualLogger;
            bool threw = false;

            std::filesystem::path testLog(L"C:\\bad\\file\\path\\test.log");
            sharedConfig.SetFile(testLog.string( ));

            try
            {
                pDualLogger = std::make_unique<DualLogger>(sharedConfig);
            }
            catch ( const std::bad_alloc& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }
            catch ( const std::runtime_error& )
            {
                threw = true;
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            try
            {
                SUTL_TEST_ASSERT(!std::filesystem::exists(testLog));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(threw);

            SUTL_TEST_SUCCESS( );
        }


        /// Positive Tests \\\

        UnitTestResult GoodPath( )
        {
            ConfigPackage sharedConfig;
            std::unique_ptr<DualLogger> pDualLogger;

            std::filesystem::path testLog(L"test.log");
            sharedConfig.SetFile(testLog.string( ));

            try
            {
                pDualLogger = std::make_unique<DualLogger>(sharedConfig);
            }
            catch ( const std::bad_alloc& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            try
            {
                SUTL_TEST_ASSERT(std::filesystem::exists(testLog));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }
            
            try
            {
                delete pDualLogger.release( );
                SUTL_CLEANUP_ASSERT(std::filesystem::remove(testLog));
            }
            catch ( const std::exception& e )
            {
                SUTL_CLEANUP_EXCEPTION(e.what( ));
            }

            SUTL_TEST_SUCCESS( );
        }

    }

    namespace SeparateConfig
    {
        /// Negative Tests \\\

        UnitTestResult FileLoggerNoFile( )
        {
            ConfigPackage stdOutConfig;
            ConfigPackage fileConfig;
            bool threw = false;

            std::unique_ptr<DualLogger> pDualLogger;

            try
            {
                pDualLogger = std::make_unique<DualLogger>(stdOutConfig, fileConfig);
            }
            catch ( const std::bad_alloc& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }
            catch ( const std::logic_error& )
            {
                threw = true;
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(threw);

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult FileLoggerBadPath( )
        {
            ConfigPackage stdOutConfig;
            ConfigPackage fileConfig;
            bool threw = false;

            std::unique_ptr<DualLogger> pDualLogger;

            std::filesystem::path testLog(L"C:\\bad\\file\\path\\test.log");
            fileConfig.SetFile(testLog.string( ));

            try
            {
                pDualLogger = std::make_unique<DualLogger>(stdOutConfig, fileConfig);
            }
            catch ( const std::bad_alloc& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }
            catch ( const std::runtime_error& )
            {
                threw = true;
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            try
            {
                SUTL_TEST_ASSERT(!std::filesystem::exists(testLog));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(threw);

            SUTL_TEST_SUCCESS( );
        }


        /// Positive Tests \\\

        UnitTestResult StdOutLoggerNoFile( )
        {
            ConfigPackage stdOutConfig;
            ConfigPackage fileConfig;
            std::unique_ptr<DualLogger> pDualLogger;

            std::filesystem::path testLog(L"test.log");
            fileConfig.SetFile(testLog.string( ));

            try
            {
                pDualLogger = std::make_unique<DualLogger>(stdOutConfig, fileConfig);
            }
            catch ( const std::bad_alloc& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            try
            {
                SUTL_TEST_ASSERT(std::filesystem::exists(testLog));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            try
            {
                delete pDualLogger.release( );
                SUTL_CLEANUP_ASSERT(std::filesystem::remove(testLog));
            }
            catch ( const std::exception& e )
            {
                SUTL_CLEANUP_EXCEPTION(e.what( ));
            }

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult StdOutLoggerBadPath( )
        {
            ConfigPackage stdOutConfig;
            ConfigPackage fileConfig;
            std::unique_ptr<DualLogger> pDualLogger;

            std::filesystem::path badLog(L"C:\\bad\\file\\path\\test.log");
            std::filesystem::path testLog(L"test.log");

            stdOutConfig.SetFile(badLog.string( ));
            fileConfig.SetFile(testLog.string( ));

            try
            {
                pDualLogger = std::make_unique<DualLogger>(stdOutConfig, fileConfig);
            }
            catch ( const std::bad_alloc& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            try
            {
                SUTL_TEST_ASSERT(!std::filesystem::exists(badLog));
                SUTL_TEST_ASSERT(std::filesystem::exists(testLog));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            try
            {
                delete pDualLogger.release( );
                SUTL_CLEANUP_ASSERT(std::filesystem::remove(testLog));
            }
            catch ( const std::exception& e )
            {
                SUTL_CLEANUP_EXCEPTION(e.what( ));
            }

            SUTL_TEST_SUCCESS( );
        }

    }

    namespace GetConfig
    {
        /// Positive Tests \\\

        UnitTestResult GetFileLoggerConfig( )
        {
            ConfigPackage stdOutConfig;
            ConfigPackage fileConfig;
            std::unique_ptr<DualLogger> pDualLogger;

            std::filesystem::path testLog(L"test_file.log");
            
            stdOutConfig.Enable(SLL::OptionFlag::LogTimestamp | SLL::OptionFlag::LogInColor);
            
            fileConfig.Enable(SLL::OptionFlag::PREFIX_MASK);
            fileConfig.SetFile(testLog.string( ));

            try
            {
                pDualLogger = std::make_unique<DualLogger>(stdOutConfig, fileConfig);
            }
            catch ( const std::bad_alloc& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                SUTL_TEST_ASSERT(std::filesystem::exists(testLog));
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                SUTL_TEST_ASSERT(pDualLogger->GetConfig<SLL::FileLogger>( ) == fileConfig);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            try
            {
                delete pDualLogger.release( );
                SUTL_CLEANUP_ASSERT(std::filesystem::remove(testLog));
            }
            catch ( const std::exception& e )
            {
                SUTL_CLEANUP_EXCEPTION(e.what( ));
            }

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult GetStdOutLoggerConfig( )
        {
            ConfigPackage stdOutConfig;
            ConfigPackage fileConfig;
            std::unique_ptr<DualLogger> pDualLogger;

            std::filesystem::path testLog(L"test_file.log");

            stdOutConfig.Enable(SLL::OptionFlag::LogTimestamp | SLL::OptionFlag::LogThreadID);

            fileConfig.Enable(SLL::OptionFlag::PREFIX_MASK);
            fileConfig.SetFile(testLog.string( ));

            try
            {
                pDualLogger.reset(new DualLogger(stdOutConfig, fileConfig));
            }
            catch ( const std::bad_alloc& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                SUTL_TEST_ASSERT(std::filesystem::exists(testLog));
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                SUTL_TEST_ASSERT(pDualLogger->GetConfig<SLL::StdOutLogger>( ) == stdOutConfig);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            try
            {
                delete pDualLogger.release( );
                SUTL_CLEANUP_ASSERT(std::filesystem::remove(testLog));
            }
            catch ( const std::exception& e )
            {
                SUTL_CLEANUP_EXCEPTION(e.what( ));
            }

            SUTL_TEST_SUCCESS( );
        }
    }
}