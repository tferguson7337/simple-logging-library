// SUTL
#include <UnitTestRunner.h>

// Enum Class Unit Tests
#include <ColorTests.h>
#include <OptionFlagTests.h>
#include <VerbosityLevelTests.h>

// Logger Unit Tests
#include <LoggerBaseTests.h>
#include <FileLoggerTests.h>


void GetUnitTests(UnitTestRunner<char>& utr)
{
    // Enum Class Unit Tests
    utr.AddUnitTests(ColorTests::GetTests( ));
    utr.AddUnitTests(OptionFlagTests::GetTests( ));
    utr.AddUnitTests(VerbosityLevelTests::GetTests( ));

    // Logger Unit Tests
    utr.AddUnitTests(LoggerBaseTests::GetTests( ));
    utr.AddUnitTests(FileLoggerTests::GetTests( ));
}

int main( )
{
    bool pass = false;

    UnitTestRunner<char> utr("SLL Unit Tests");

    // Get Tests
    GetUnitTests(utr);

    // Run Tests
    pass = utr.RunUnitTests( );

    // Print Results
    utr.PrintTestLogs( );

    return (pass) ? 0 : -1;
}
