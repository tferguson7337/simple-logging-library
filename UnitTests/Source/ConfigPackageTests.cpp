#pragma once

#include <ConfigPackageTests.h>

#include <ConfigPackage.h>

#include <LoggerBaseTests.h>
#include <FileLoggerTests.h>
#include <StdOutLoggerTests.h>

namespace ConfigPackageTests
{
    using SLL::ConfigPackage;

    using SLL::Color;
    using SLL::OptionFlag;
    using SLL::VerbosityLevel;

    std::list<std::function<UnitTestResult(void)>> GetTests( )
    {
        static const std::list<std::function<UnitTestResult(void)>> testList
        {
            // Comparsion Tests

            /// Positive Tests \\\

            Comparison::EquivalencyOptionFlags,
            Comparison::EquivalencyVerbosityTheshold,
            Comparison::EquivalencyColors,
            Comparison::EquivalencyLogFile,

            Comparison::NonequivalencyOptionFlags,
            Comparison::NonequivalencyVerbosityTheshold,
            Comparison::NonequivalencyColors,
            Comparison::NonequivalencyLogFile,


            // GetColor Tests

            /// Negative Test \\\

            GetColor::InvalidVerbosityLevel,

            /// Positive Test \\\

            GetColor::ValidVerbosityLevel,


            // SetColor Tests

            /// Negative Tests \\\

            SetColor::InvalidVerbosityLevel,
            SetColor::InvalidColor,

            /// Positive Test \\\

            SetColor::ValidVerbosityLevelAndColor,


            // OptionFlags Tests

            /// Negative Tests \\\

            OptionFlags::EnableInvalidOptionFlag,
            OptionFlags::EnableInvalidOptionFlags,

            OptionFlags::DisableInvalidOptionFlag,
            OptionFlags::DisableInvalidOptionFlags,

            OptionFlags::IsEnabledInvalidOptionFlag,
            OptionFlags::IsEnabledInvalidOptionFlags,

            OptionFlags::IsEnabledAnyInvalidOptionFlag,
            OptionFlags::IsEnabledAnyInvalidOptionFlags,

            OptionFlags::IsEnabledAllInvalidOptionFlag,
            OptionFlags::IsEnabledAllInvalidOptionFlags,

            /// Positive Tests \\\

            OptionFlags::EnableNewOption,
            OptionFlags::EnableNewOptions,

            OptionFlags::EnableAlreadyEnabledOption,
            OptionFlags::EnableAlreadyEnabledOptions,

            OptionFlags::DisableNewOption,
            OptionFlags::DisableNewOptions,

            OptionFlags::DisableAlreadyDisabledOption,
            OptionFlags::DisableAlreadyDisabledOptions,

            OptionFlags::IsEnabledOption,
            OptionFlags::IsEnabledOptions,

            OptionFlags::IsEnabledAnyOption,
            OptionFlags::IsEnabledAnyOptions,

            OptionFlags::IsEnabledAllOption,
            OptionFlags::IsEnabledAllOptions,


            // SetVerbosityThreshold Tests

            /// Negative Test \\\

            SetVerbosityThreshold::InvalidVerbosityLevel,

            /// Positive Test \\\

            SetVerbosityThreshold::ValidVerbosityLevel
        };

        return testList;
    }

    namespace Comparison
    {
        /// Positive Tests \\\

        UnitTestResult EquivalencyOptionFlags( )
        {
            ConfigPackage configL;
            ConfigPackage configR;

            for ( OptionFlag flagL = OptionFlag::BEGIN, flagR = OptionFlag::NONE; flagL < OptionFlag::MAX; INCREMENT_OPTIONFLAG(flagL) )
            {
                configL.Disable(OptionFlag::ALL_MASK);
                configR.Disable(OptionFlag::ALL_MASK);

                SUTL_TEST_ASSERT(configL == configR);

                configL.Enable(flagL);
                SUTL_TEST_ASSERT(!(configL == configR));

                INCREMENT_OPTIONFLAG(flagR);
                configR.Enable(flagR);
                SUTL_TEST_ASSERT(configL == configR);
            }

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult EquivalencyVerbosityTheshold( )
        {
            ConfigPackage configL;
            ConfigPackage configR;
            
            for ( VerbosityLevel lvlL = static_cast<VerbosityLevel>(static_cast<SLL::VerbosityLevelType>(VerbosityLevel::BEGIN) + 1), lvlR = VerbosityLevel::BEGIN; lvlL < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvlL) )
            {
                SUTL_TEST_ASSERT(configL == configR);

                configL.SetVerbosityThreshold(lvlL);
                SUTL_TEST_ASSERT(!(configL == configR));

                INCREMENT_VERBOSITY(lvlR);
                configR.SetVerbosityThreshold(lvlR);
                SUTL_TEST_ASSERT(configL == configR);
            }

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult EquivalencyColors( )
        {
            ConfigPackage configL;
            ConfigPackage configR;

            for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
            {
                SUTL_TEST_ASSERT(configL == configR);

                for ( Color clr = Color::BASIC_BEGIN; clr < Color::MAX; INCREMENT_COLOR(clr) )
                {
                    configL.SetColor(lvl, clr);
                    SUTL_TEST_ASSERT(!(configL == configR));

                    configR.SetColor(lvl, clr);
                    SUTL_TEST_ASSERT(configL == configR);
                }
            }

            SUTL_TEST_SUCCESS( );
        }


        UnitTestResult EquivalencyLogFile( )
        {
            ConfigPackage configL;
            ConfigPackage configR;

            SUTL_TEST_ASSERT(configL == configR);

            configL.SetFile(L"C:\\test.log");

            SUTL_TEST_ASSERT(!(configL == configR));

            configR.SetFile(L"C:\\TEST.LOG");

            SUTL_TEST_ASSERT(!(configL == configR));

            configR.SetFile(L"C:\\test.log");

            SUTL_TEST_ASSERT(configL == configR);

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult NonequivalencyOptionFlags( )
        {
            ConfigPackage configL;
            ConfigPackage configR;

            for ( OptionFlag flagL = OptionFlag::BEGIN, flagR = OptionFlag::NONE; flagL < OptionFlag::MAX; INCREMENT_OPTIONFLAG(flagL) )
            {
                configL.Disable(OptionFlag::ALL_MASK);
                configR.Disable(OptionFlag::ALL_MASK);

                SUTL_TEST_ASSERT(!(configL != configR));

                configL.Enable(flagL);
                SUTL_TEST_ASSERT(configL != configR);

                INCREMENT_OPTIONFLAG(flagR);
                configR.Enable(flagR);
                SUTL_TEST_ASSERT(!(configL != configR));
            }

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult NonequivalencyVerbosityTheshold( )
        {
            ConfigPackage configL;
            ConfigPackage configR;

            for ( VerbosityLevel lvlL = static_cast<VerbosityLevel>(static_cast<SLL::VerbosityLevelType>(VerbosityLevel::BEGIN) + 1), lvlR = VerbosityLevel::BEGIN; lvlL < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvlL) )
            {
                SUTL_TEST_ASSERT(!(configL != configR));

                configL.SetVerbosityThreshold(lvlL);
                SUTL_TEST_ASSERT(configL != configR);

                INCREMENT_VERBOSITY(lvlR);
                configR.SetVerbosityThreshold(lvlR);
                SUTL_TEST_ASSERT(!(configL != configR));
            }

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult NonequivalencyColors( )
        {
            ConfigPackage configL;
            ConfigPackage configR;

            for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
            {
                SUTL_TEST_ASSERT(!(configL != configR));

                for ( Color clr = Color::BASIC_BEGIN; clr < Color::MAX; INCREMENT_COLOR(clr) )
                {
                    configL.SetColor(lvl, clr);
                    SUTL_TEST_ASSERT(configL != configR);

                    configR.SetColor(lvl, clr);
                    SUTL_TEST_ASSERT(!(configL != configR));
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult NonequivalencyLogFile( )
        {
            ConfigPackage configL;
            ConfigPackage configR;

            SUTL_TEST_ASSERT(!(configL != configR));

            configL.SetFile(L"C:\\test.log");

            SUTL_TEST_ASSERT(configL != configR);

            configR.SetFile(L"C:\\TEST.LOG");

            SUTL_TEST_ASSERT(configL != configR);

            configR.SetFile(L"C:\\test.log");

            SUTL_TEST_ASSERT(!(configL != configR));

            SUTL_TEST_SUCCESS( );
        }
    }

    namespace GetColor
    {
        /// Negative Test \\\

        UnitTestResult InvalidVerbosityLevel( )
        {
            ConfigPackage config;
            bool threw = false;

            try
            {
                config.GetColor(VerbosityLevel::MAX);
            }
            catch ( const std::invalid_argument& )
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


        /// Positive Test \\\

        UnitTestResult ValidVerbosityLevel( )
        {
            ConfigPackage config;

            try
            {
                config.SetColor(VerbosityLevel::INFO, Color::BRIGHT | Color::GREEN);
                config.SetColor(VerbosityLevel::WARN, Color::BRIGHT | Color::YELLOW);
                config.SetColor(VerbosityLevel::ERROR, Color::BRIGHT | Color::RED);
                config.SetColor(VerbosityLevel::FATAL, Color::BRIGHT | Color::CYAN);
            }
            catch ( const std::exception& e )
            {
                SUTL_SETUP_EXCEPTION(e.what( ));
            }

            try
            {
                SUTL_TEST_ASSERT(config.GetColor(VerbosityLevel::INFO) == (Color::BRIGHT | Color::GREEN));
                SUTL_TEST_ASSERT(config.GetColor(VerbosityLevel::WARN) == (Color::BRIGHT | Color::YELLOW));
                SUTL_TEST_ASSERT(config.GetColor(VerbosityLevel::ERROR) == (Color::BRIGHT | Color::RED));
                SUTL_TEST_ASSERT(config.GetColor(VerbosityLevel::FATAL) == (Color::BRIGHT | Color::CYAN));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_SUCCESS( );
        }
    }

    namespace SetColor
    {
        /// Negative Tests \\\

        UnitTestResult InvalidVerbosityLevel( )
        {
            ConfigPackage config;
            bool threw = false;

            try
            {
                config.SetColor(VerbosityLevel::MAX, Color::GREEN);
            }
            catch ( const std::invalid_argument& )
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

        UnitTestResult InvalidColor( )
        {
            ConfigPackage config;
            bool threw = false;

            try
            {
                config.SetColor(VerbosityLevel::INFO, Color::MAX);
            }
            catch ( const std::invalid_argument& )
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


        /// Positive Test \\\

        UnitTestResult ValidVerbosityLevelAndColor( )
        {
            ConfigPackage config;

            try
            {
                config.SetColor(VerbosityLevel::INFO, Color::BRIGHT | Color::GREEN);
                config.SetColor(VerbosityLevel::WARN, Color::BRIGHT | Color::YELLOW);
                config.SetColor(VerbosityLevel::ERROR, Color::BRIGHT | Color::RED);
                config.SetColor(VerbosityLevel::FATAL, Color::BRIGHT | Color::CYAN);
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            try
            {
                SUTL_TEST_ASSERT(config.GetColor(VerbosityLevel::INFO) == (Color::BRIGHT | Color::GREEN));
                SUTL_TEST_ASSERT(config.GetColor(VerbosityLevel::WARN) == (Color::BRIGHT | Color::YELLOW));
                SUTL_TEST_ASSERT(config.GetColor(VerbosityLevel::ERROR) == (Color::BRIGHT | Color::RED));
                SUTL_TEST_ASSERT(config.GetColor(VerbosityLevel::FATAL) == (Color::BRIGHT | Color::CYAN));
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_SUCCESS( );
        }
    }

    namespace OptionFlags
    {
        /// Negative Test \\\

        UnitTestResult EnableInvalidOptionFlag( )
        {
            ConfigPackage config;
            bool threw = false;

            try
            {
                config.Enable(OptionFlag::MAX);
            }
            catch ( const std::invalid_argument& )
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

        UnitTestResult EnableInvalidOptionFlags( )
        {
            ConfigPackage config;
            bool threw = false;

            try
            {
                config.Enable(OptionFlag::LogAsynchronous | OptionFlag::MAX | static_cast<OptionFlag>(1337));
            }
            catch ( const std::invalid_argument& )
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


        UnitTestResult DisableInvalidOptionFlag( )
        {
            ConfigPackage config;
            bool threw = false;

            try
            {
                config.Disable(OptionFlag::MAX);
            }
            catch ( const std::invalid_argument& )
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

        UnitTestResult DisableInvalidOptionFlags( )
        {
            ConfigPackage config;
            bool threw = false;

            try
            {
                config.Disable(OptionFlag::LogAsynchronous | OptionFlag::MAX | static_cast<OptionFlag>(1337));
            }
            catch ( const std::invalid_argument& )
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


        UnitTestResult IsEnabledInvalidOptionFlag( )
        {
            ConfigPackage config;
            bool threw = false;

            try
            {
                config.OptionEnabled(OptionFlag::MAX);
            }
            catch ( const std::invalid_argument& )
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

        UnitTestResult IsEnabledInvalidOptionFlags( )
        {
            ConfigPackage config;
            bool threw = false;

            try
            {
                config.OptionEnabled(OptionFlag::LogAsynchronous | OptionFlag::MAX | static_cast<OptionFlag>(1337));
            }
            catch ( const std::invalid_argument& )
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


        UnitTestResult IsEnabledAnyInvalidOptionFlag( )
        {
            ConfigPackage config;
            bool threw = false;

            try
            {
                config.OptionsEnabledAny(OptionFlag::MAX);
            }
            catch ( const std::invalid_argument& )
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

        UnitTestResult IsEnabledAnyInvalidOptionFlags( )
        {
            ConfigPackage config;
            bool threw = false;

            try
            {
                config.OptionsEnabledAny(OptionFlag::LogAsynchronous | OptionFlag::MAX | static_cast<OptionFlag>(1337));
            }
            catch ( const std::invalid_argument& )
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

        UnitTestResult IsEnabledAllInvalidOptionFlag( )
        {
            ConfigPackage config;
            bool threw = false;

            try
            {
                config.OptionsEnabledAll(OptionFlag::MAX);
            }
            catch ( const std::invalid_argument& )
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

        UnitTestResult IsEnabledAllInvalidOptionFlags( )
        {
            ConfigPackage config;
            bool threw = false;

            try
            {
                config.OptionsEnabledAll(OptionFlag::LogAsynchronous | OptionFlag::MAX | static_cast<OptionFlag>(1337));
            }
            catch ( const std::invalid_argument& )
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


        /// Positive Tests \\\

        UnitTestResult EnableNewOption( )
        {
            ConfigPackage config;

            for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; f <<= 1 )
            {
                config.Disable(OptionFlag::ALL_MASK);

                config.Enable(f);

                SUTL_TEST_ASSERT(config.GetOptionFlags( ) == f);
            }

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult EnableNewOptions( )
        {
            ConfigPackage config;

            for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; INCREMENT_OPTIONFLAG(f) )
            {
                config.Disable(OptionFlag::ALL_MASK);

                config.Enable(f);

                SUTL_TEST_ASSERT(config.GetOptionFlags( ) == f);
            }

            SUTL_TEST_SUCCESS( );
        }


        UnitTestResult EnableAlreadyEnabledOption( )
        {
            ConfigPackage config;

            config.Enable(OptionFlag::ALL_MASK);

            for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; f <<= 1 )
            {
                config.Enable(f);

                SUTL_TEST_ASSERT(config.GetOptionFlags( ) == OptionFlag::ALL_MASK);
            }

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult EnableAlreadyEnabledOptions( )
        {
            ConfigPackage config;

            config.Enable(OptionFlag::ALL_MASK);

            for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; INCREMENT_OPTIONFLAG(f) )
            {
                config.Enable(f);

                SUTL_TEST_ASSERT(config.GetOptionFlags( ) == OptionFlag::ALL_MASK);
            }

            SUTL_TEST_SUCCESS( );
        }


        UnitTestResult DisableNewOption( )
        {
            ConfigPackage config;

            for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; f <<= 1 )
            {
                config.Enable(OptionFlag::ALL_MASK);

                config.Disable(f);

                SUTL_TEST_ASSERT(config.GetOptionFlags( ) == ~f);
            }

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult DisableNewOptions( )
        {
            ConfigPackage config;

            for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; INCREMENT_OPTIONFLAG(f) )
            {
                config.Enable(OptionFlag::ALL_MASK);

                config.Disable(f);

                SUTL_TEST_ASSERT(config.GetOptionFlags( ) == ~f);
            }

            SUTL_TEST_SUCCESS( );
        }


        UnitTestResult DisableAlreadyDisabledOption( )
        {
            ConfigPackage config;

            for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; f <<= 1 )
            {
                config.Enable(~f);

                config.Disable(f);

                SUTL_TEST_ASSERT(config.GetOptionFlags( ) == ~f);
            }

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult DisableAlreadyDisabledOptions( )
        {
            ConfigPackage config;

            for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::ALL_MASK; INCREMENT_OPTIONFLAG(f) )
            {
                config.Enable(~f);

                config.Disable(f);

                SUTL_TEST_ASSERT(config.GetOptionFlags( ) == ~f);
            }

            SUTL_TEST_SUCCESS( );
        }


        UnitTestResult IsEnabledOption( )
        {
            ConfigPackage config;

            for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; f <<= 1 )
            {
                for ( OptionFlag e = OptionFlag::BEGIN; e < OptionFlag::MAX; e <<= 1 )
                {
                    config.Disable(OptionFlag::ALL_MASK);
                    config.Enable(e);

                    if ( f == e )
                    {
                        SUTL_TEST_ASSERT(config.OptionEnabled(f));
                    }
                    else
                    {
                        SUTL_TEST_ASSERT(!config.OptionEnabled(f));
                    }
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult IsEnabledOptions( )
        {
            ConfigPackage config;

            for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; INCREMENT_OPTIONFLAG(f) )
            {
                for ( OptionFlag e = OptionFlag::BEGIN; e < OptionFlag::MAX; INCREMENT_OPTIONFLAG(e) )
                {
                    config.Disable(OptionFlag::ALL_MASK);
                    config.Enable(e);

                    if ( (f & e) == f )
                    {
                        SUTL_TEST_ASSERT(config.OptionEnabled(f));
                    }
                    else
                    {
                        SUTL_TEST_ASSERT(!config.OptionEnabled(f));
                    }
                }
            }

            SUTL_TEST_SUCCESS( );
        }


        UnitTestResult IsEnabledAnyOption( )
        {
            ConfigPackage config;

            for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; f <<= 1 )
            {
                for ( OptionFlag e = OptionFlag::BEGIN; e < OptionFlag::MAX; e <<= 1 )
                {
                    config.Disable(OptionFlag::ALL_MASK);
                    config.Enable(e);

                    if ( (f & e) != OptionFlag::NONE )
                    {
                        SUTL_TEST_ASSERT(config.OptionsEnabledAny(f));
                    }
                    else
                    {
                        SUTL_TEST_ASSERT(!config.OptionsEnabledAny(f));
                    }
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult IsEnabledAnyOptions( )
        {
            ConfigPackage config;

            for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; INCREMENT_OPTIONFLAG(f) )
            {
                for ( OptionFlag e = OptionFlag::BEGIN; e < OptionFlag::MAX; INCREMENT_OPTIONFLAG(e) )
                {
                    config.Disable(OptionFlag::ALL_MASK);
                    config.Enable(e);

                    if ( (f & e) != OptionFlag::NONE )
                    {
                        SUTL_TEST_ASSERT(config.OptionsEnabledAny(f));
                    }
                    else
                    {
                        SUTL_TEST_ASSERT(!config.OptionsEnabledAny(f));
                    }
                }
            }

            SUTL_TEST_SUCCESS( );
        }


        UnitTestResult IsEnabledAllOption( )
        {
            ConfigPackage config;

            for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; f <<= 1 )
            {
                for ( OptionFlag e = OptionFlag::BEGIN; e < OptionFlag::MAX; e <<= 1 )
                {
                    config.Disable(OptionFlag::ALL_MASK);
                    config.Enable(e);

                    if ( (f & e) == f )
                    {
                        SUTL_TEST_ASSERT(config.OptionsEnabledAll(f));
                    }
                    else
                    {
                        SUTL_TEST_ASSERT(!config.OptionsEnabledAll(f));
                    }
                }
            }

            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult IsEnabledAllOptions( )
        {
            ConfigPackage config;

            for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; INCREMENT_OPTIONFLAG(f) )
            {
                for ( OptionFlag e = OptionFlag::BEGIN; e < OptionFlag::MAX; INCREMENT_OPTIONFLAG(e) )
                {
                    config.Disable(OptionFlag::ALL_MASK);
                    config.Enable(e);

                    if ( (f & e) == f )
                    {
                        SUTL_TEST_ASSERT(config.OptionsEnabledAll(f));
                    }
                    else
                    {
                        SUTL_TEST_ASSERT(!config.OptionsEnabledAll(f));
                    }
                }
            }

            SUTL_TEST_SUCCESS( );
        }
    }

    namespace SetVerbosityThreshold
    {
        /// Negative Test \\\

        UnitTestResult InvalidVerbosityLevel( )
        {
            ConfigPackage config;
            bool threw = false;

            try
            {
                config.SetVerbosityThreshold(VerbosityLevel::MAX);
            }
            catch ( const std::invalid_argument& )
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


        /// Positive Test \\\

        UnitTestResult ValidVerbosityLevel( )
        {
            ConfigPackage config;

            for ( VerbosityLevel lvl = VerbosityLevel::BEGIN; lvl < VerbosityLevel::MAX; INCREMENT_VERBOSITY(lvl) )
            {
                try
                {
                    config.SetVerbosityThreshold(lvl);
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }

                SUTL_TEST_ASSERT(config.GetVerbosityThreshold( ) == lvl);
            }

            SUTL_TEST_SUCCESS( );
        }
    }
}