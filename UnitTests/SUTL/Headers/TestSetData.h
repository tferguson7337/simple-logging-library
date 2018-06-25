#ifndef _TEST_SET_DATA_H
#define _TEST_SET_DATA_H

#include <Types.h>
#include <ITestSetData.h>

template <class T>
class TestSetData : public virtual ITestSetData<T>
{
private:
    uint64 mTotalTests;
    uint64 mTestPassCount;
    uint64 mTotalFailureCount;
    uint64 mSetupFailureCount;
    uint64 mTestFailureCount;
    uint64 mCleanupFailureCount;
    uint64 mSetupExceptionCount;
    uint64 mTestExceptionCount;
    uint64 mCleanupExceptionCount;
    uint64 mUnhandledExceptionCount;
    uint64 mTestSkipCount;

    std::basic_string<T> mTestSetName;

public:
    /// Ctors \\\

    explicit TestSetData(const std::basic_string<T>& = std::basic_string<T>( )) noexcept;
    explicit TestSetData(std::basic_string<T>&&) noexcept;
    TestSetData(const TestSetData&) noexcept;
    TestSetData(TestSetData&&) noexcept;

    /// Dtor \\\

    ~TestSetData( ) = default;

    /// Assignment Overloads \\\

    TestSetData& operator=(const TestSetData&) noexcept;
    TestSetData& operator=(TestSetData&&) noexcept;

    /// Getters \\\

    uint64 GetTotalTestCount( ) const noexcept;
    uint64 GetTestPassCount( ) const noexcept;
    uint64 GetTotalFailureCount( ) const noexcept;
    uint64 GetSetupFailureCount( ) const noexcept;
    uint64 GetTestFailureCount( ) const noexcept;
    uint64 GetCleanupFailureCount( ) const noexcept;
    uint64 GetSetupExceptionCount( ) const noexcept;
    uint64 GetTestExceptionCount( ) const noexcept;
    uint64 GetCleanupExceptionCount( ) const noexcept;
    uint64 GetUnhandledExceptionCount( ) const noexcept;
    uint64 GetTestSkipCount( ) const noexcept;

    const std::basic_string<T>& GetTestSetName( ) const noexcept;

    /// Incrementers \\\

    void IncrementResultCounter(Result);

    /// Setters \\\

    void SetTotalTestCount(uint64) noexcept;

    void SetTestSetName(const std::basic_string<T>&);
    void SetTestSetName(std::basic_string<T>&&) noexcept;

    /// Public Methods \\\

    void ResetCounters( ) noexcept;
    void ClearAll( ) noexcept;
    long double GetTestSetGrade( ) const noexcept;
};

#endif