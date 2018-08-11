#pragma once

// Logger Interface
#include <Interfaces\ILogger.h>

// STL - std::thread::id
#include <thread>


// Forward declaration of unit-testing helper-class.
namespace LoggerBaseTests
{
    class Tester;
}

namespace SLL
{
    ///
    //
    //  Class   - LoggerBase
    //
    //  Purpose - Implements logging-related functionality common to all
    //            logging classes intended to be derived from this class.
    //
    ///
    class LoggerBase : public virtual ILogger
    {
        // Friend class, intended to expose non-public methods for testing.
        friend class ::LoggerBaseTests::Tester;

    private:

        /// Common Private Data Member \\\

        ConfigPackage mConfig;

        /// Common Private Helper Methods \\\

        // Prefix - Timestamp Format Getter (std::put_time Format)
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static const std::basic_string<T>& GetTimeFormat( );

        // Prefix - Thread ID Format Getter
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static const std::basic_string<T>& GetThreadIDFormat( );

        // Prefix - VerbosityLevel Format Getter
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static const std::basic_string<T>& GetVerbosityLevelFormat( );

        // Extract Thread ID
        static unsigned long ExtractThreadID(const std::thread::id&);

        // Get Local Time String
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static std::basic_string<T> GetLocalTime( );

        // Build Time Prefix String
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static std::unique_ptr<T[ ]> BuildTimePrefix( );

        // Returns required length of buffer to hold built format-string
        // that would be built using format and arguments.
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static size_t GetRequiredBufferLength(const T*, va_list);

        // Will build format-string with arguments, filling the passed buffer.
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static std::unique_ptr<T[ ]> StringPrintWrapper(const size_t, const T*, va_list);

    protected:

        /// Constructors \\\

        // ConfigPackage Constructor [C]
        explicit LoggerBase(const ConfigPackage&);

        // ConfigPackage Constructor [M]
        explicit LoggerBase(ConfigPackage&&) noexcept;

        // Move Constructor
        LoggerBase(LoggerBase&&) noexcept;

        /// Destructor \\\

        // Virtual Destructor
        virtual ~LoggerBase( ) = default;

        /// Assignment Overload \\\

        // Move Assignment
        LoggerBase& operator=(LoggerBase&&);

        /// Common Protected Helper Methods \\\

        // Generates log-ready string that contains all enabled message-prefix output.
        // e.g., timestamp (mm/dd/yyyy, HH:mm::ss), thread id, verbosity level, etc.
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        std::vector<std::unique_ptr<T[ ]>> BuildMessagePrefixes(const VerbosityLevel& lvl, const std::thread::id& tid) const;

        // Build user's formatted log message (w/ va_list).
        template<class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static std::unique_ptr<T[ ]> BuildFormattedMessage(const T* pFormat, va_list args);

    public:

        /// Common Public Helper Methods \\\

        // Build user's formatted log message (w/o va_list).
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static std::unique_ptr<T[ ]> BuildFormattedMessage(const T* pFormat, ...);

        /// Common Getter \\\

        const ConfigPackage& GetConfig( ) const noexcept;
    };
}