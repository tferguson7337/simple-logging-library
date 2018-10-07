#include <ColorTests.h>

#include <Color.h>

namespace ColorTests
{
    using SLL::Color;
    using SLL::ColorType;
    using SLL::ColorConverter;

    // Helper Macros
#define COLOR_TO_TYPE(c) (static_cast<ColorType>(c))
#define TYPE_TO_COLOR(t) (static_cast<Color>(t))
#define INCREMENT_COLOR(c) ( c = TYPE_TO_COLOR(COLOR_TO_TYPE(c) + 1) )
#define DECREMENT_COLOR(c) ( c = TYPE_TO_COLOR(COLOR_TO_TYPE(c) - 1) )

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
        for ( Color c = Color::BASIC_BEGIN; c < Color::BASIC_END; INCREMENT_COLOR(c) )
        {
            SUTL_TEST_ASSERT(~c == TYPE_TO_COLOR(COLOR_TO_TYPE(Color::BASIC_END) - COLOR_TO_TYPE(c) - 1));
        }

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult BitwiseOr( )
    {
        Color clr = Color::BASIC_BEGIN;
        for ( Color c = Color::BASIC_BEGIN; c < Color::BASIC_END; INCREMENT_COLOR(c) )
        {
            SUTL_TEST_ASSERT((clr | c) == c);
        }

        clr = TYPE_TO_COLOR(COLOR_TO_TYPE(Color::BASIC_END) - 1);
        SUTL_TEST_ASSERT((clr | Color::BASIC_BEGIN) == clr);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult BitwiseAnd( )
    {
        Color clr = TYPE_TO_COLOR(COLOR_TO_TYPE(Color::BASIC_END) - 1);
        for ( Color c = Color::BASIC_BEGIN; c < Color::BASIC_END; INCREMENT_COLOR(c) )
        {
            SUTL_TEST_ASSERT((clr & c) == c);
        }

        SUTL_TEST_ASSERT((clr & Color::BASIC_BEGIN) == Color::BASIC_BEGIN);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult BitwiseXor( )
    {
        Color clr = Color::BASIC_BEGIN;
        for ( Color c = Color::BASIC_BEGIN; c < Color::BASIC_END; INCREMENT_COLOR(c) )
        {
            SUTL_TEST_ASSERT((clr ^ c) == c);
        }

        clr = TYPE_TO_COLOR(COLOR_TO_TYPE(Color::BASIC_END) - 1);
        SUTL_TEST_ASSERT((clr ^ Color::BASIC_BEGIN) == clr);
        SUTL_TEST_ASSERT((clr ^ TYPE_TO_COLOR(COLOR_TO_TYPE(Color::BASIC_END) - 1)) == Color::BASIC_BEGIN);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }


    // Bitwise Assignment Operator Tests \\

    UnitTestResult BitwiseOrAssign( )
    {
        Color clr = Color::BASIC_BEGIN;
        for ( Color c = Color::BASIC_BEGIN; c < Color::BASIC_END; INCREMENT_COLOR(c) )
        {
            const Color oldFlag = clr;
            SUTL_TEST_ASSERT((clr |= c) == (oldFlag | c));
        }

        clr |= Color::BASIC_BEGIN;
        SUTL_TEST_ASSERT(clr == TYPE_TO_COLOR(COLOR_TO_TYPE(Color::BASIC_END) - 1));

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult BitwiseAndAssign( )
    {
        Color clr = TYPE_TO_COLOR(COLOR_TO_TYPE(Color::BASIC_END) - 1);
        for ( Color c = TYPE_TO_COLOR(COLOR_TO_TYPE(Color::BASIC_END) - 1); c != Color::BASIC_BEGIN; DECREMENT_COLOR(c) )
        {
            const Color oldFlag = clr;
            SUTL_TEST_ASSERT((clr &= c) == (oldFlag & c));
        }

        clr &= TYPE_TO_COLOR(COLOR_TO_TYPE(Color::BASIC_END) - 1);
        SUTL_TEST_ASSERT(clr == Color::BASIC_BEGIN);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult BitwiseXorAssign( )
    {
        Color clr = Color::BASIC_BEGIN;
        for ( Color c = Color::BASIC_BEGIN; c < Color::BASIC_END; INCREMENT_COLOR(c) )
        {
            const Color oldFlag = clr;
            SUTL_TEST_ASSERT((clr ^= c) == (oldFlag ^ c));
        }

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }


    // Shift Operator Tests \\

    UnitTestResult LeftShift( )
    {
        Color clr = Color::RED;
        ColorType val = COLOR_TO_TYPE(Color::RED) << 1;
        ColorType s = 1;

        for ( ; (clr << s) < Color::BASIC_END; s++, val <<= 1 )
        {
            SUTL_TEST_ASSERT((clr << s) == TYPE_TO_COLOR(val));
        }

        SUTL_TEST_ASSERT((clr << s) == Color::BASIC_END);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult RightShift( )
    {
        Color clr = Color::BASIC_END;
        ColorType val = COLOR_TO_TYPE(Color::BASIC_END) >> 1;
        ColorType s = 1;

        for ( ; (clr >> s) > Color::BASIC_BEGIN; s++, val >>= 1 )
        {
            SUTL_TEST_ASSERT((clr >> s) == TYPE_TO_COLOR(val));
        }

        SUTL_TEST_ASSERT((clr >> s) == Color::BASIC_BEGIN);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }


    // Shift Assignment Operator Tests \\

    UnitTestResult LeftShiftAssign( )
    {
        Color clr = Color::RED;
        ColorType val = COLOR_TO_TYPE(Color::RED);

        for ( ; clr < Color::BASIC_END; clr <<= 1, val <<= 1 )
        {
            SUTL_TEST_ASSERT((clr) == TYPE_TO_COLOR(val));
        }

        SUTL_TEST_ASSERT((clr <<= 1) == Color::BASIC_END);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }

    UnitTestResult RightShiftAssign( )
    {
        Color clr = Color::BASIC_END;
        ColorType val = COLOR_TO_TYPE(Color::BASIC_END);

        for ( ; clr > Color::BASIC_BEGIN; clr >>= 1, val >>= 1 )
        {
            SUTL_TEST_ASSERT((clr) == TYPE_TO_COLOR(val));
        }

        SUTL_TEST_ASSERT((clr >>= 1) == Color::BASIC_BEGIN);

        /// Test Pass!
        SUTL_TEST_SUCCESS( );
    }
    namespace ConverterTests
    {
        UnitTestResult ToScalar( )
        {
            Color c = Color::BASIC_BEGIN;
            bool threw = false;

            for ( ; c <= Color::MAX; INCREMENT_COLOR(c) )
            {
                try
                {
                    SUTL_TEST_ASSERT(ColorConverter::ToScalar(c) == COLOR_TO_TYPE(c));
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }
            }

            try
            {
                ColorConverter::ToScalar(c);
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
        UnitTestResult ToString( )
        {
            std::basic_string<T> str;
            Color c = Color::BASIC_BEGIN;
            bool threw = false;

            for ( ; c < Color::MAX; INCREMENT_COLOR(c) )
            {
                try
                {
                    str = ColorConverter::ToString<T>(c);
                    str.clear( );
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }
            }

            try
            {
                str = ColorConverter::ToString<T>(c);
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