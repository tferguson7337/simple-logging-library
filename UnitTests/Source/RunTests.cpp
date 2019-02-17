// SUTL
#include <UnitTestRunner.h>

// Enum Class Unit Tests
#include <ColorTests.h>
#include <OptionFlagTests.h>
#include <VerbosityLevelTests.h>

// ConfigPackage Unit Tests
#include <ConfigPackageTests.h>

// Logger Unit Tests
#include <LoggerBaseTests.h>
#include <FileLoggerTests.h>
#include <StdOutLoggerTests.h>
#include <DualLoggerTests.h>
#include <AsyncLoggerTests.h>

void GetUnitTests(UnitTestRunner<utf8>& utr)
{
    // Enum Class Unit Tests
    utr.AddUnitTests(ColorTests::GetTests( ));
    utr.AddUnitTests(OptionFlagTests::GetTests( ));
    utr.AddUnitTests(VerbosityLevelTests::GetTests( ));

    // ConfigPackage Unit Tests
    utr.AddUnitTests(ConfigPackageTests::GetTests( ));

    // Logger Unit Tests
    utr.AddUnitTests(LoggerBaseTests::GetTests( ));
    utr.AddUnitTests(FileLoggerTests::GetTests( ));
    utr.AddUnitTests(StdOutLoggerTests::GetTests( ));
    utr.AddUnitTests(DualLoggerTests::GetTests( ));
    utr.AddUnitTests(AsyncLoggerTests::GetTests( ));
}

int main( )
{
    bool pass = false;

    UnitTestRunner<utf8> utr("SLL Unit Tests");

    // Get Tests
    GetUnitTests(utr);

    // Run Tests
    pass = utr.RunUnitTests( );

    return (pass) ? 0 : -1;

    return 0;
}
