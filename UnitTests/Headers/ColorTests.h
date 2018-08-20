#pragma once

#include <Macros.h>

#include <functional>
#include <list>

#include <UnitTestResult.h>

namespace ColorTests
{
    std::list<std::function<UnitTestResult(void)>> GetTests( );

    // Bitwise Operator Tests
    UnitTestResult BitwiseNot( );
    UnitTestResult BitwiseOr( );
    UnitTestResult BitwiseAnd( );
    UnitTestResult BitwiseXor( );

    // Bitwise Assignment Operator Tests
    UnitTestResult BitwiseOrAssign( );
    UnitTestResult BitwiseAndAssign( );
    UnitTestResult BitwiseXorAssign( );

    // Shift Operator Tests
    UnitTestResult LeftShift( );
    UnitTestResult RightShift( );

    // Shift Assignment Operator Tests
    UnitTestResult LeftShiftAssign( );
    UnitTestResult RightShiftAssign( );

    // Conversion Tests
    namespace ConverterTests
    {
        UnitTestResult ToScalar( );

        template <class T, ENABLE_IF_SUPPORTED_CHARACTER_TYPE(T)>
        UnitTestResult ToString( );
    }
}