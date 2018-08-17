#pragma once

#include <Macros.h>

#include <functional>
#include <list>

#include <UnitTestResult.h>

namespace VerbosityLevelTests
{
    std::list<std::function<UnitTestResult(void)>> GetTests( );

    namespace ConverterTests
    {
        UnitTestResult ToScalar( );
        
        template <class T, ENABLE_IF_SUPPORTED_CHARACTER_TYPE(T)>
        UnitTestResult ToString( );
    }
}