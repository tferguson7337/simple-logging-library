#pragma once

#include <UnitTestResult.h>

#include <functional>
#include <list>

namespace ConfigPackageTests
{
    std::list<std::function<UnitTestResult(void)>> GetTests( );

    namespace Comparison
    {
        /// Positive Tests \\\

        UnitTestResult EquivalencyOptionFlags( );
        UnitTestResult EquivalencyVerbosityTheshold( );
        UnitTestResult EquivalencyColors( );
        UnitTestResult EquivalencyLogFile( );

        UnitTestResult NonequivalencyOptionFlags( );
        UnitTestResult NonequivalencyVerbosityTheshold( );
        UnitTestResult NonequivalencyColors( );
        UnitTestResult NonequivalencyLogFile( );
    }

    namespace GetColor
    {
        /// Negative Test \\\

        UnitTestResult InvalidVerbosityLevel( );

        /// Positive Test \\\

        UnitTestResult ValidVerbosityLevel( );
    }

    namespace SetColor
    {
        /// Negative Tests \\\

        UnitTestResult InvalidVerbosityLevel( );
        UnitTestResult InvalidColor( );

        /// Positive Test \\\

        UnitTestResult ValidVerbosityLevelAndColor( );
    }

    namespace OptionFlags
    {
        /// Negative Tests \\\

        UnitTestResult EnableInvalidOptionFlag( );
        UnitTestResult EnableInvalidOptionFlags( );

        UnitTestResult DisableInvalidOptionFlag( );
        UnitTestResult DisableInvalidOptionFlags( );

        UnitTestResult IsEnabledInvalidOptionFlag( );
        UnitTestResult IsEnabledInvalidOptionFlags( );

        UnitTestResult IsEnabledAnyInvalidOptionFlag( );
        UnitTestResult IsEnabledAnyInvalidOptionFlags( );

        UnitTestResult IsEnabledAllInvalidOptionFlag( );
        UnitTestResult IsEnabledAllInvalidOptionFlags( );

        /// Positive Tests \\\

        UnitTestResult EnableNewOption( );
        UnitTestResult EnableNewOptions( );

        UnitTestResult EnableAlreadyEnabledOption( );
        UnitTestResult EnableAlreadyEnabledOptions( );

        UnitTestResult DisableNewOption( );
        UnitTestResult DisableNewOptions( );

        UnitTestResult DisableAlreadyDisabledOption( );
        UnitTestResult DisableAlreadyDisabledOptions( );

        UnitTestResult IsEnabledOption( );
        UnitTestResult IsEnabledOptions( );

        UnitTestResult IsEnabledAnyOption( );
        UnitTestResult IsEnabledAnyOptions( );

        UnitTestResult IsEnabledAllOption( );
        UnitTestResult IsEnabledAllOptions( );
    }

    namespace SetVerbosityThreshold
    {
        /// Negative Test \\\

        UnitTestResult InvalidVerbosityLevel( );

        /// Positive Test \\\

        UnitTestResult ValidVerbosityLevel( );
    }
}