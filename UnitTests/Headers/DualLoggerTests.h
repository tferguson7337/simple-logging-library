#pragma once

#include <UnitTestResult.h>

#include <functional>
#include <list>

namespace DualLoggerTests
{
    std::list<std::function<UnitTestResult(void)>> GetTests( );

    namespace SharedConfig
    {
        /// Negative Tests \\\

        UnitTestResult NoFile( );
        UnitTestResult BadPath( );

        /// Positive Tests \\\

        UnitTestResult GoodPath( );
    }

    namespace SeparateConfig
    {
        /// Negative Tests \\\

        UnitTestResult FileLoggerNoFile( );
        UnitTestResult FileLoggerBadPath( );

        /// Positive Tests \\\

        UnitTestResult StdOutLoggerNoFile( );
        UnitTestResult StdOutLoggerBadPath( );
    }

    namespace GetConfig
    {
        /// Positive Tests \\\

        UnitTestResult GetFileLoggerConfig( );
        UnitTestResult GetStdOutLoggerConfig( );
    }
}