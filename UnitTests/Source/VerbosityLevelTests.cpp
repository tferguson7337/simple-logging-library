#include <VerbosityLevelTests.h>

#include <VerbosityLevel.h>

namespace VerbosityLevelTests
{
    using SLL::VerbosityLevel;
    using SLL::VerbosityLevelConverter;
    using SLL::VerbosityLevelType;

    // Helper Macros
#define VERBTOTYPE(v) (static_cast<VerbosityLevelType>(v))
#define TYPETOVERB(t) (static_cast<VerbosityLevel>(t))

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
            VerbosityLevel v = VerbosityLevel::BEGIN;
            bool threw = false;

            for ( ; v <= VerbosityLevel::MAX; v = TYPETOVERB(VERBTOTYPE(v) + 1) )
            {
                try
                {
                    SUTL_TEST_ASSERT(VerbosityLevelConverter::ToScalar(v) == VERBTOTYPE(v));
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }
            }

            try
            {
                VerbosityLevelConverter::ToScalar(v);
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
            VerbosityLevel v = VerbosityLevel::BEGIN;
            bool threw = false;

            for ( ; v < VerbosityLevel::MAX; v = TYPETOVERB(VERBTOTYPE(v) + 1) )
            {
                try
                {
                    str = VerbosityLevelConverter::ToString<T>(v);
                    str.clear( );
                }
                catch ( const std::exception& e )
                {
                    SUTL_TEST_EXCEPTION(e.what( ));
                }
            }

            try
            {
                str = VerbosityLevelConverter::ToString<T>(v);
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