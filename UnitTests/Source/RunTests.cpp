// SUTL
#include <UnitTestRunner.h>

// Unit Tests
#include <OptionFlagTests.h>

void GetUnitTests(UnitTestRunner<char>&);

int main( )
{
    UnitTestRunner<char> utr("Simple-Logging-Library Unit Tests");

    GetUnitTests(utr);

    utr.RunUnitTests( );

    utr.PrintTestLogs( );

    return 0;
}

void GetUnitTests(UnitTestRunner<char>& utr)
{
    utr.AddUnitTests(OptionFlagTests::GetTests( ));
}