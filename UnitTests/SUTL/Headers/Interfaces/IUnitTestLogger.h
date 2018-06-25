#ifndef _UNIT_TEST_LOGGER_INTERFACE_H
#define _UNIT_TEST_LOGGER_INTERFACE_H

#include <string>

/// Forward Declaration
template <class T>
class UnitTestLogger;

template <class T>
class TestSetData;

class UnitTestResult;

template <class T>
class IUnitTestLogger
{
public:
    IUnitTestLogger( ) = default;
    virtual ~IUnitTestLogger( ) = default;

    virtual UnitTestLogger<T>& operator=(UnitTestLogger<T>&&) noexcept = 0;

    virtual const std::basic_string<T>& GetTargetFile( ) const = 0;

    virtual bool SetTargetFile(const std::basic_string<T>&) = 0;
    virtual bool SetTargetFile(std::basic_string<T>&&) = 0;

    virtual bool LogTestSetHeader(const TestSetData<T>&) = 0;
    virtual bool LogUnitTestResult(const UnitTestResult&) = 0;
    virtual bool LogTestSetSummary(const TestSetData<T>&) = 0;
};

#endif