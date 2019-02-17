#pragma once

#include <UnitTestResult.h>

#include <functional>
#include <list>

namespace AsyncLoggerTests
{
    std::list<std::function<UnitTestResult(void)>> GetTests( );

    namespace Log
    {
        template <class T>
        UnitTestResult LogAsynchronously( );
    }
}