#ifndef _RESULT_ENUMS_H
#define _RESULT_ENUMS_H

/// Unit Test Result Enum Flags \\\

enum class Result
{
    // Test not run
    NotRun,

    // Test pass
    Success,

    // Test failures - setup
    SetupFailure,
    SetupException,

    // Test failures - actual
    TestFailure,
    TestException,

    // Test failures - cleanup
    CleanupFailure,
    CleanupException,

    // Test failures - unhandled exception.
    UnhandledException
};

#endif // _RESULT_ENUMS_H