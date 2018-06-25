#ifndef _TEST_SET_DATA_INTERFACE_H
#define _TEST_SET_DATA_INTERFACE_H

#include "ResultEnums.h"
#include "Types.h"

#include <string>

/// Forward Declaration
template <class T>
class TestSetData;

template <class T>
class ITestSetData
{
public:
    ITestSetData( ) = default;
    virtual ~ITestSetData( ) = default;

    /// Assignment Overloads \\\

    virtual TestSetData<T>& operator=(const TestSetData<T>&) noexcept = 0;
    virtual TestSetData<T>& operator=(TestSetData<T>&&) noexcept = 0;

    /// Getters \\\

    virtual uint64 GetTotalTestCount( ) const noexcept = 0;
    virtual uint64 GetTestPassCount( ) const noexcept = 0;
    virtual uint64 GetTotalFailureCount( ) const noexcept = 0;
    virtual uint64 GetSetupFailureCount( ) const noexcept = 0;
    virtual uint64 GetTestFailureCount( ) const noexcept = 0;
    virtual uint64 GetCleanupFailureCount( ) const noexcept = 0;
    virtual uint64 GetSetupExceptionCount( ) const noexcept = 0;
    virtual uint64 GetTestExceptionCount( ) const noexcept = 0;
    virtual uint64 GetCleanupExceptionCount( ) const noexcept = 0;
    virtual uint64 GetUnhandledExceptionCount( ) const noexcept = 0;
    virtual uint64 GetTestSkipCount( ) const noexcept = 0;

    virtual const std::basic_string<T>& GetTestSetName( ) const noexcept = 0;

    /// Incrementers \\\

    virtual void IncrementResultCounter(Result) = 0;

    /// Setters \\\

    virtual void SetTotalTestCount(uint64) noexcept = 0;

    virtual void SetTestSetName(const std::basic_string<T>&) = 0;
    virtual void SetTestSetName(std::basic_string<T>&&) noexcept = 0;

    /// Public Methods \\\

    virtual void ResetCounters( ) noexcept = 0;
    virtual void ClearAll( ) noexcept = 0;
    virtual long double GetTestSetGrade( ) const noexcept = 0;
};

#endif