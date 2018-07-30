// SUTL
#include <UnitTestRunner.h>

// Enum Class Unit Tests
#include <ColorTests.h>
#include <OptionFlagTests.h>
#include <VerbosityLevelTests.h>


void GetUnitTests(UnitTestRunner<char>& utr)
{
    // Enum Class Tests
    utr.AddUnitTests(ColorTests::GetTests( ));
    utr.AddUnitTests(OptionFlagTests::GetTests( ));
    utr.AddUnitTests(VerbosityLevelTests::GetTests( ));
}

int main( )
{
    UnitTestRunner<char> utr("SLL Unit Tests");

    // Get Tests
    GetUnitTests(utr);

    // Run Tests
    utr.RunUnitTests( );

    // Print Results
    utr.PrintTestLogs( );

    return 0;
}
