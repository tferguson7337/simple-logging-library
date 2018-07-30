#pragma once

#include <functional>
#include <list>

#include <UnitTestResult.h>

namespace ColorTests
{
    std::list<std::function<UnitTestResult(void)>> GetTests( );

    namespace ConverterTests
    {
        UnitTestResult ToScalar( );

        template <class T, typename = std::enable_if_t<std::is_same_v<T, char> || std::is_same_v<T, wchar_t>>>
        UnitTestResult ToString( );
    }
}