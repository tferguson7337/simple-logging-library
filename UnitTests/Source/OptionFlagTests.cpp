#include <OptionFlag.h>

#include <OptionFlagTests.h>

namespace OptionFlagTests
{
    using SLL::OptionFlag;
    using SLL::OptionFlagType;
    using SLL::OptionFlagConverter;

    // Helper Macros
#define FLAGTOTYPE(f) (static_cast<OptionFlagType>(f))
#define TYPETOFLAG(t) (static_cast<OptionFlag>(t))

    std::list<std::function<UnitTestResult(void)>> GetTests( )
    {
        static const std::list<std::function<UnitTestResult(void)>> testList
        {
            // Bitwise Operator Tests
            BitwiseNot,
            BitwiseOr,
            BitwiseAnd,
            BitwiseXor,

            // Bitwise Assignment Operator Tests
            BitwiseOrAssign,
            BitwiseAndAssign,
            BitwiseXorAssign,

            // Shift Operator Tests
            LeftShift,
            RightShift,

            // Shift Assignment Operator Tests
            LeftShiftAssign,
            RightShiftAssign,

            // Conversion Tests
            ConverterTests::ToScalar,
            ConverterTests::ToString_Narrow,
            ConverterTests::ToString_Wide,
        };

        return testList;
    }

    // Bitwise Operator Tests \\

    UnitTestResult BitwiseNot( )
    {
        for ( OptionFlag f = OptionFlag::NONE; f < OptionFlag::MAX; f = TYPETOFLAG(FLAGTOTYPE(f) + 1) )
        {
            SUTL_TEST_ASSERT(~f == TYPETOFLAG(FLAGTOTYPE(OptionFlag::MAX) - FLAGTOTYPE(f) - 1));
        }

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult BitwiseOr( )
    {
        OptionFlag flag = OptionFlag::NONE;
        for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; f = TYPETOFLAG(FLAGTOTYPE(f) + 1) )
        {
            SUTL_TEST_ASSERT((flag | f) == f);
        }

        flag = TYPETOFLAG(FLAGTOTYPE(OptionFlag::MAX) - 1);
        SUTL_TEST_ASSERT((flag | OptionFlag::NONE) == flag);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult BitwiseAnd( )
    {
        OptionFlag flag = TYPETOFLAG(FLAGTOTYPE(OptionFlag::MAX) - 1);
        for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; f = TYPETOFLAG(FLAGTOTYPE(f) + 1) )
        {
            SUTL_TEST_ASSERT((flag & f) == f);
        }

        SUTL_TEST_ASSERT((flag & OptionFlag::NONE) == OptionFlag::NONE);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult BitwiseXor( )
    {
        OptionFlag flag = OptionFlag::NONE;
        for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; f = TYPETOFLAG(FLAGTOTYPE(f) + 1) )
        {
            SUTL_TEST_ASSERT((flag ^ f) == f);
        }

        flag = TYPETOFLAG(FLAGTOTYPE(OptionFlag::MAX) - 1);
        SUTL_TEST_ASSERT((flag ^ OptionFlag::NONE) == flag);
        SUTL_TEST_ASSERT((flag ^ TYPETOFLAG(FLAGTOTYPE(OptionFlag::MAX) - 1)) == OptionFlag::NONE);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }


    // Bitwise Assignment Operator Tests \\

    UnitTestResult BitwiseOrAssign( )
    {
        OptionFlag flag = OptionFlag::NONE;
        for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; f = TYPETOFLAG(FLAGTOTYPE(f) + 1) )
        {
            const OptionFlag oldFlag = flag;
            SUTL_TEST_ASSERT((flag |= f) == (oldFlag | f));
        }

        flag |= OptionFlag::NONE;
        SUTL_TEST_ASSERT(flag == TYPETOFLAG(FLAGTOTYPE(OptionFlag::MAX) - 1));

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult BitwiseAndAssign( )
    {
        OptionFlag flag = TYPETOFLAG(FLAGTOTYPE(OptionFlag::MAX) - 1);
        for ( OptionFlag f = TYPETOFLAG(FLAGTOTYPE(OptionFlag::MAX) - 1); f != OptionFlag::NONE; f = TYPETOFLAG(FLAGTOTYPE(f) - 1) )
        {
            const OptionFlag oldFlag = flag;
            SUTL_TEST_ASSERT((flag &= f) == (oldFlag & f));
        }

        flag &= TYPETOFLAG(FLAGTOTYPE(OptionFlag::MAX) - 1);
        SUTL_TEST_ASSERT(flag == OptionFlag::NONE);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult BitwiseXorAssign( )
    {
        OptionFlag flag = OptionFlag::NONE;
        for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; f = TYPETOFLAG(FLAGTOTYPE(f) + 1) )
        {
            const OptionFlag oldFlag = flag;
            SUTL_TEST_ASSERT((flag ^= f) == (oldFlag ^ f));
        }

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }


    // Shift Operator Tests \\

    UnitTestResult LeftShift( )
    {
        OptionFlag flag = OptionFlag::BEGIN;
        OptionFlagType val = FLAGTOTYPE(OptionFlag::BEGIN) << 1;
        OptionFlagType s = 1;

        for ( ; (flag << s) < OptionFlag::MAX; s++, val <<= 1 )
        {
            SUTL_TEST_ASSERT((flag << s) == TYPETOFLAG(val));
        }

        SUTL_TEST_ASSERT((flag << s) == OptionFlag::MAX);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult RightShift( )
    {
        OptionFlag flag = OptionFlag::MAX;
        OptionFlagType val = FLAGTOTYPE(OptionFlag::MAX) >> 1;
        OptionFlagType s = 1;

        for ( ; (flag >> s) > OptionFlag::NONE; s++, val >>= 1 )
        {
            SUTL_TEST_ASSERT((flag >> s) == TYPETOFLAG(val));
        }

        SUTL_TEST_ASSERT((flag >> s) == OptionFlag::NONE);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }


    // Shift Assignment Operator Tests \\

    UnitTestResult LeftShiftAssign( )
    {
        OptionFlag flag = OptionFlag::BEGIN;
        OptionFlagType val = FLAGTOTYPE(OptionFlag::BEGIN);

        for ( ; flag < OptionFlag::MAX; flag <<= 1, val <<= 1 )
        {
            SUTL_TEST_ASSERT((flag) == TYPETOFLAG(val));
        }

        SUTL_TEST_ASSERT((flag <<= 1) == OptionFlag::MAX);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult RightShiftAssign( )
    {
        OptionFlag flag = OptionFlag::MAX;
        OptionFlagType val = FLAGTOTYPE(OptionFlag::MAX);

        for ( ; flag > OptionFlag::NONE; flag >>= 1, val >>= 1 )
        {
            SUTL_TEST_ASSERT((flag) == TYPETOFLAG(val));
        }

        SUTL_TEST_ASSERT((flag >>= 1) == OptionFlag::NONE);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }


    namespace ConverterTests
    {
        // Conversion Tests \\

        UnitTestResult ToScalar( )
        {
            OptionFlag flag = OptionFlag::NONE;
            bool threw = false;

            for ( ; flag <= OptionFlag::MAX; flag = TYPETOFLAG(FLAGTOTYPE(flag) + 1) )
            {
                try
                {
                    SUTL_TEST_ASSERT(OptionFlagConverter::ToScalar(flag) == FLAGTOTYPE(flag));
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }
            }

            try
            {
                OptionFlagConverter::ToScalar(flag);
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_TEST_ASSERT(threw);

            /// Test Pass!
            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult ToString_Narrow( )
        {
            std::basic_string<char> str;
            bool threw = false;

            for ( OptionFlag f = OptionFlag::NONE; f < OptionFlag::MAX; f = TYPETOFLAG(FLAGTOTYPE(f) + 1) )
            {
                try
                {
                    str = OptionFlagConverter::ToString<char>(f);
                    str.clear( );
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }
            }
            
            try
            {
                str = OptionFlagConverter::ToString<char>(OptionFlag::MAX);
                str.clear( );
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_TEST_ASSERT(threw);

            /// Test Pass!
            SUTL_TEST_SUCCESS( );
        }

        UnitTestResult ToString_Wide( )
        {
            std::basic_string<wchar_t> str;
            bool threw = false;

            for ( OptionFlag f = OptionFlag::NONE; f < OptionFlag::MAX; f = TYPETOFLAG(FLAGTOTYPE(f) + 1) )
            {
                try
                {
                    str = OptionFlagConverter::ToString<wchar_t>(f);
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }
            }

            try
            {
                str = OptionFlagConverter::ToString<wchar_t>(OptionFlag::MAX);
                str.clear( );
            }
            catch ( const std::exception& )
            {
                threw = true;
            }

            SUTL_TEST_ASSERT(threw);

            /// Test Pass!
            SUTL_TEST_SUCCESS( );
        }
    }
}