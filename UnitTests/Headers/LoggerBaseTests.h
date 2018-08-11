#pragma once

#include <LoggerBase.h>

#include <Uncopyable.h>
#include <Unmovable.h>

#include <cstdarg>

#include <functional>
#include <list>

#include <UnitTestResult.h>


namespace LoggerBaseTests
{
    std::list<std::function<UnitTestResult(void)>> GetTests( );

    // Format Getter Tests
    UnitTestResult GetTimeFormat( );
    UnitTestResult GetThreadIDFormat( );
    UnitTestResult GetVerbosityLevelFormat( );
    
    // Build Time Prefix Tests
    UnitTestResult GetLocalTime( );
    UnitTestResult BuildTimePrefix( );

    // Get Required Buffer Length Tests
    namespace GetRequiredBufferLengthTests
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult NoFormat( );

        /// Positive Tests \\\

        template <class T>
        UnitTestResult NoArgs( );

        template <class T>
        UnitTestResult IntegralArgs( );

        template <class T>
        UnitTestResult FloatingArgs( );

        template <class T>
        UnitTestResult StringArgs( );

        template <class T>
        UnitTestResult TimeFormat( );

        template <class T>
        UnitTestResult ThreadIDFormat( );

        template <class T>
        UnitTestResult VerbosityLevelFormat( );
    }
    
    // String Print Wrapper Tests
    namespace StringPrintWrapperTests
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult ZeroLength( );

        template <class T>
        UnitTestResult NoFormat( );

        /// Positive Tests \\\

        template <class T>
        UnitTestResult NoArgs( );

        template <class T>
        UnitTestResult IntegralArgs( );

        template <class T>
        UnitTestResult FloatingArgs( );

        template <class T>
        UnitTestResult StringArgs( );

        template <class T>
        UnitTestResult ThreadIDFormat( );

        template <class T>
        UnitTestResult VerbosityLevelFormat( );
    }

    // Build Message Prefix(es) Tests
    namespace BuildMessagePrefixesTests
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult BadVerbosityLevel( );

        /// Positive Tests \\\

        template <class T>
        UnitTestResult NoOptions( );
        
        template <class T>
        UnitTestResult TimeOnly( );

        template <class T>
        UnitTestResult ThreadIDOnly( );

        template <class T>
        UnitTestResult VerbosityLevelOnly( );

        template <class T>
        UnitTestResult Time_ThreadID( );

        template <class T>
        UnitTestResult Time_VerbosityLevel( );

        template <class T>
        UnitTestResult ThreadID_VerbosityLevel( );

        template <class T>
        UnitTestResult AllOptions( );
    }

    // Build Formatted String Tests
    namespace BuildFormattedMessageTests
    {
        /// Negative Tests \\\

        template <class T>
        UnitTestResult NoFormat( );

        /// Positive Tests \\\

        template <class T>
        UnitTestResult NoArgs( );

        template <class T>
        UnitTestResult IntegralArgs( );

        template <class T>
        UnitTestResult FloatingArgs( );

        template <class T>
        UnitTestResult StringArgs( );
    }
}


namespace SLL
{
    namespace LoggerBaseTests
    {
        ///
        //
        //  Class   - LoggerBaseTester
        //
        //  Purpose - Friend of LoggerBase, exposes non-public methods for testing.
        //
        ///
        class Tester final : public LoggerBase
        {
            Tester(const Tester&) = delete;
            Tester(Tester&&) = delete;
            Tester& operator=(const Tester&) = delete;
            Tester& operator=(Tester&&) = delete;

        public:
            /// Constructors \\\
            
            explicit Tester(const ConfigPackage& cp) :
                LoggerBase(cp)
            { }

            explicit Tester(ConfigPackage&& cp) noexcept :
                LoggerBase(std::move(cp))
            { }

            /// Destructors \\\

            virtual ~Tester( ) = default;

            /// LoggerBase Static Wrappers \\\

            // Prefix - Timestamp Format Getter (std::put_time Format)
            template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
            static const std::basic_string<T>& GetTimeFormat( )
            {
                return LoggerBase::GetTimeFormat<T>( );
            }

            // Prefix - Thread ID Format Getter
            template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
            static const std::basic_string<T>& GetThreadIDFormat( )
            {
                return LoggerBase::GetThreadIDFormat<T>( );
            }

            // Prefix - VerbosityLevel Format Getter
            template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
            static const std::basic_string<T>& GetVerbosityLevelFormat( )
            {
                return LoggerBase::GetVerbosityLevelFormat<T>( );
            }

            // Extract Thread ID
            static unsigned long ExtractThreadID(const std::thread::id& tid)
            {
                return LoggerBase::ExtractThreadID(tid);
            }

            // Get Local Time String
            template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
            static std::basic_string<T> GetLocalTime( )
            {
                return LoggerBase::GetLocalTime<T>( );
            }

            // Build Time Prefix String
            template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
            static std::unique_ptr<T[ ]> BuildTimePrefix( )
            {
                return LoggerBase::BuildTimePrefix<T>( );
            }

            // Returns required length of buffer to hold built format-string
            // that would be built using format and arguments.
            template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
            static size_t GetRequiredBufferLength(const T* f, ...)
            {
                size_t l = 0;
                va_list a;
                va_start(a, f);
                try
                {
                    l = LoggerBase::GetRequiredBufferLength<T>(f, a);
                }
                catch ( const std::invalid_argument& )
                {
                    va_end(a);
                    throw;
                }
                catch ( const std::exception& )
                {
                    va_end(a);
                    throw;
                }
                 
                va_end(a);
                return l;
            }

            // Will build format-string with arguments, filling the passed buffer.
            template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
            static std::unique_ptr<T[ ]> StringPrintWrapper(const size_t len, const T* f, ...)
            {
                std::unique_ptr<T[ ]> str;
                va_list a;

                va_start(a, f);

                try
                {
                    str = LoggerBase::StringPrintWrapper<T>(len, f, a);
                }
                catch ( const std::invalid_argument& )
                {
                    va_end(a);
                    throw;
                }
                catch ( const std::exception& )
                {
                    va_end(a);
                    throw;
                }

                va_end(a);

                return str;
            }

            /// LoggerBase Non-Static Wrappers \\\

            // Wrapper for BuildMessagePrefixes.
            template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
            std::vector<std::unique_ptr<T[ ]>> BuildMessagePrefixes(const VerbosityLevel& lvl, const std::thread::id& tid) const
            {
                return LoggerBase::BuildMessagePrefixes<T>(lvl, tid);
            }

            // Dummy Flush method to satisfy ILogger interface.
            void Flush( )
            {
                __noop;
            }
        };
    }
}
