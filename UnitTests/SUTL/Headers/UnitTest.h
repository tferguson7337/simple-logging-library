#ifndef _UNIT_TEST_H
#define _UNIT_TEST_H

#include <Uncopyable.h>

#include <IUnitTest.h>
#include <UnitTestResult.h>

#include <memory>

///
//
//  Class:      UnitTest
//  
//  Purpose:    Encapsulate unit test information.
//
///
class UnitTest final : public IUnitTest, public Uncopyable
{
private:
    /// Private Data Members \\\

    std::unique_ptr<std::function<UnitTestResult(void)>> mTestFuncPtr;
    UnitTestResult mTestResult;

public:
    /// Ctors \\\

    explicit UnitTest(const std::function<UnitTestResult(void)>&);
    explicit UnitTest(UnitTest&&) noexcept;

    /// Dtor \\\

    ~UnitTest( ) = default;

    /// Operator Overloads \\\

    UnitTest& operator=(UnitTest&&) noexcept;

    /// Getters \\\

    std::function<UnitTestResult(void)> GetUnitTestFunction( ) const;
    const UnitTestResult& GetUnitTestResult( ) const noexcept;

    /// Setters \\\

    void SetUnitTestFunction(std::function<UnitTestResult(void)>);

    /// Public Methods \\\

    const UnitTestResult& RunTest( );
};

#endif // _UNIT_TEST_H
