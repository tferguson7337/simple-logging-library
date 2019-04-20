#include <OptionFlag.h>

#include <OptionFlagTests.h>

#include <stdexcept>

namespace OptionFlagTests
{
    using SLL::OptionFlag;
    using SLL::OptionFlagType;
    using SLL::OptionFlagConverter;

    // Helper Macros
#define FLAG_TO_TYPE(f) (static_cast<OptionFlagType>(f))
#define TYPE_TO_FLAG(t) (static_cast<OptionFlag>(t))
#define INCREMENT_FLAG(f) (f = TYPE_TO_FLAG(FLAG_TO_TYPE(f) + 1))
#define DECREMENT_FLAG(f) (f = TYPE_TO_FLAG(FLAG_TO_TYPE(f) - 1))

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
            ConverterTests::ToString<utf8>,
            ConverterTests::ToString<utf16>
        };

        return testList;
    }

    // Bitwise Operator Tests \\

    UnitTestResult BitwiseNot( )
    {
        for ( OptionFlag f = OptionFlag::NONE; f < OptionFlag::MAX; INCREMENT_FLAG(f) )
        {
            SUTL_TEST_ASSERT(~f == TYPE_TO_FLAG(FLAG_TO_TYPE(OptionFlag::MAX) - FLAG_TO_TYPE(f) - 1));
        }

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult BitwiseOr( )
    {
        OptionFlag flag = OptionFlag::NONE;
        for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; INCREMENT_FLAG(f) )
        {
            SUTL_TEST_ASSERT((flag | f) == f);
        }

        flag = TYPE_TO_FLAG(FLAG_TO_TYPE(OptionFlag::MAX) - 1);
        SUTL_TEST_ASSERT((flag | OptionFlag::NONE) == flag);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult BitwiseAnd( )
    {
        OptionFlag flag = TYPE_TO_FLAG(FLAG_TO_TYPE(OptionFlag::MAX) - 1);
        for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; INCREMENT_FLAG(f) )
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
        for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; INCREMENT_FLAG(f) )
        {
            SUTL_TEST_ASSERT((flag ^ f) == f);
        }

        flag = TYPE_TO_FLAG(FLAG_TO_TYPE(OptionFlag::MAX) - 1);
        SUTL_TEST_ASSERT((flag ^ OptionFlag::NONE) == flag);
        SUTL_TEST_ASSERT((flag ^ TYPE_TO_FLAG(FLAG_TO_TYPE(OptionFlag::MAX) - 1)) == OptionFlag::NONE);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }


    // Bitwise Assignment Operator Tests \\

    UnitTestResult BitwiseOrAssign( )
    {
        OptionFlag flag = OptionFlag::NONE;
        for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; INCREMENT_FLAG(f) )
        {
            const OptionFlag oldFlag = flag;
            SUTL_TEST_ASSERT((flag |= f) == (oldFlag | f));
        }

        flag |= OptionFlag::NONE;
        SUTL_TEST_ASSERT(flag == TYPE_TO_FLAG(FLAG_TO_TYPE(OptionFlag::MAX) - 1));

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult BitwiseAndAssign( )
    {
        OptionFlag flag = TYPE_TO_FLAG(FLAG_TO_TYPE(OptionFlag::MAX) - 1);
        for ( OptionFlag f = TYPE_TO_FLAG(FLAG_TO_TYPE(OptionFlag::MAX) - 1); f != OptionFlag::NONE; DECREMENT_FLAG(f) )
        {
            const OptionFlag oldFlag = flag;
            SUTL_TEST_ASSERT((flag &= f) == (oldFlag & f));
        }

        flag &= TYPE_TO_FLAG(FLAG_TO_TYPE(OptionFlag::MAX) - 1);
        SUTL_TEST_ASSERT(flag == OptionFlag::NONE);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult BitwiseXorAssign( )
    {
        OptionFlag flag = OptionFlag::NONE;
        for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX; INCREMENT_FLAG(f) )
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
        OptionFlagType val = FLAG_TO_TYPE(OptionFlag::BEGIN) << 1;
        OptionFlagType s = 1;

        for ( ; (flag << s) < OptionFlag::MAX; s++, val <<= 1 )
        {
            SUTL_TEST_ASSERT((flag << s) == TYPE_TO_FLAG(val));
        }

        SUTL_TEST_ASSERT((flag << s) == OptionFlag::MAX);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult RightShift( )
    {
        OptionFlag flag = OptionFlag::MAX;
        OptionFlagType val = FLAG_TO_TYPE(OptionFlag::MAX) >> 1;
        OptionFlagType s = 1;

        for ( ; (flag >> s) > OptionFlag::NONE; s++, val >>= 1 )
        {
            SUTL_TEST_ASSERT((flag >> s) == TYPE_TO_FLAG(val));
        }

        SUTL_TEST_ASSERT((flag >> s) == OptionFlag::NONE);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }


    // Shift Assignment Operator Tests \\

    UnitTestResult LeftShiftAssign( )
    {
        OptionFlag flag = OptionFlag::BEGIN;
        OptionFlagType val = FLAG_TO_TYPE(OptionFlag::BEGIN);

        for ( ; flag < OptionFlag::MAX; flag <<= 1, val <<= 1 )
        {
            SUTL_TEST_ASSERT((flag) == TYPE_TO_FLAG(val));
        }

        SUTL_TEST_ASSERT((flag <<= 1) == OptionFlag::MAX);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult RightShiftAssign( )
    {
        OptionFlag flag = OptionFlag::MAX;
        OptionFlagType val = FLAG_TO_TYPE(OptionFlag::MAX);

        for ( ; flag > OptionFlag::NONE; flag >>= 1, val >>= 1 )
        {
            SUTL_TEST_ASSERT((flag) == TYPE_TO_FLAG(val));
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
            OptionFlag f = OptionFlag::NONE;
            bool threw = false;

            for ( ; f <= OptionFlag::MAX; INCREMENT_FLAG(f) )
            {
                try
                {
                    SUTL_TEST_ASSERT(OptionFlagConverter::ToScalar(f) == FLAG_TO_TYPE(f));
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }
            }

            try
            {
                OptionFlagConverter::ToScalar(f);
            }
            catch ( const std::invalid_argument& )
            {
                threw = true;
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(threw);

            /// Test Pass!
            SUTL_TEST_SUCCESS( );
        }

        template <class T, typename>
        inline UnitTestResult ToString( )
        {
            std::basic_string<T> str;
            bool threw = false;

            for ( OptionFlag f = OptionFlag::NONE; f < OptionFlag::MAX; INCREMENT_FLAG(f) )
            {
                try
                {
                    str = OptionFlagConverter::ToString<T>(f);
                    str.clear( );
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }
            }

            try
            {
                str = OptionFlagConverter::ToString<T>(OptionFlag::MAX);
                str.clear( );
            }
            catch ( const std::invalid_argument& )
            {
                threw = true;
            }
            catch ( const std::exception& e )
            {
                SUTL_TEST_EXCEPTION(e.what( ));
            }

            SUTL_TEST_ASSERT(threw);

            /// Test Pass!
            SUTL_TEST_SUCCESS( );
        }
    }
}