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

    std::list<std::function<UnitTestResult(void)>> GetTests( )
    {
        static const std::list<std::function<UnitTestResult(void)>> testList
        {
            ConverterTests::ToScalar,
            ConverterTests::ToString<char>,
            ConverterTests::ToString<wchar_t>
        };

        return testList;
    }

    namespace ConverterTests
    {
        UnitTestResult ToScalar( )
        {
            Color c = Color::BEGIN;
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
            Color c = Color::BEGIN;
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