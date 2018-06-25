#ifndef _UNIT_TEST_RESULT_INTERFACE_H
#define _UNIT_TEST_RESULT_INTERFACE_H

#include <string>

#include "ResultEnums.h"
#include "Types.h"

/// Forward Declaration
class UnitTestResult;

///
//
//  Class:      IUnitTestResult
//  
//  Purpose:    Define public interface for UnitTestResult class.
//
///
class IUnitTestResult
{
public:
    // Ctor / Dtor
    IUnitTestResult( ) = default;
    virtual ~IUnitTestResult( ) = default;

    virtual UnitTestResult& operator=(UnitTestResult&&) noexcept = 0;

    /// Operator Overloads \\\

    virtual explicit operator bool( ) const = 0;

    /// Getters \\\

    virtual Result GetResult( ) const = 0;
    virtual const std::string& GetFunctionName( ) const = 0;
    virtual const std::string& GetFileName( ) const = 0;
    virtual const std::string& GetResultInfo( ) const = 0;
    virtual uint64 GetLineNumber( ) const = 0;

    /// Setters \\\

    virtual void SetResult(Result) = 0;
    virtual void SetLineNumber(uint64) = 0;
    virtual void SetFunctionName(const std::string&) = 0;
    virtual void SetFileName(const std::string&) = 0;
    virtual void SetResultInfo(const std::string&) = 0;
};

#endif // _UNIT_TEST_RESULT_INTERFACE_H