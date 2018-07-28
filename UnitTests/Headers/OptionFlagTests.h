#pragma once

#include <functional>
#include <list>

#include <UnitTestResult.h>

namespace OptionFlagTests
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

    namespace ConverterTests
    {
        // Conversion Tests
        UnitTestResult ToScalar( );
        UnitTestResult ToString_Narrow( );
        UnitTestResult ToString_Wide( );
    }
}
