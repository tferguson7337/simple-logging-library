#pragma once

// Logger Interface
#include "Interfaces\ILogger.h"

// Variadic Arguments
#include <cstdarg>

// Forward declaration of unit-testing helper-class.
namespace LoggerBaseTests
{
    class Tester;
}

namespace StreamLoggerTests
{
    template <class LoggerType, class StreamType>
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
        // Friend classes, intended to expose non-public methods for testing.
        friend class ::LoggerBaseTests::Tester;

        template <class LoggerType, class StreamType>
        friend class ::StreamLoggerTests::Tester;

    private:
        /// Common Private Data Member \\\

        ConfigPackage mConfig;

        /// Common Static Private Helper Methods \\\

        // Prefix - Timestamp Format Getter (std::put_time Format)
        template <class T>
        static const std::basic_string<T>& GetTimeFormat( );

        // Prefix - Thread ID Format Getter
        template <class T>
        static const std::basic_string<T>& GetThreadIDFormat( );

        // Prefix - VerbosityLevel Format Getter
        template <class T>
        static const std::basic_string<T>& GetVerbosityLevelFormat( );

        // Extract Thread ID
        static unsigned long ExtractThreadID(const std::thread::id&);

        // Get Local Time String
        template <class T>
        static std::basic_string<T> GetLocalTime( );

        // Build Time Prefix String
        template <class T>
        static std::unique_ptr<T[ ]> BuildTimePrefix( );

        // Returns required length of buffer to hold built format-string
        // that would be built using format and arguments.
        template <class T>
        static size_t GetRequiredBufferLength(const T*, va_list);

        // Will build format-string with arguments, returning the resulting string.
        template <class T>
        static std::unique_ptr<T[ ]> StringPrintWrapper(const size_t, const T*, va_list);

    protected:

        /// Common Protected Data Members \\\

        static const size_t mFlushInterval;
        size_t mFlushCounter;

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

        /// Common Protected Getters \\\

        ConfigPackage& GetConfig( ) noexcept;
        const ConfigPackage& GetConfig( ) const noexcept;

        /// Common Protected Helper Methods \\\

        // Generates log-ready string that contains all enabled message-prefix output.
        // e.g., timestamp (mm/dd/yyyy, HH:mm::ss), thread id, verbosity level, etc.
        template <class T, ENABLE_IF_SUPPORTED_CHARACTER_TYPE(T)>
        std::vector<std::unique_ptr<T[ ]>> BuildMessagePrefixes(const VerbosityLevel& lvl, const std::thread::id& tid) const;

        // Build user's formatted log message (w/ va_list).
        template<class T, ENABLE_IF_SUPPORTED_CHARACTER_TYPE(T)>
        static std::unique_ptr<T[ ]> BuildFormattedMessage(const T* pFormat, va_list args);

        // Build user's formatted log message (w/o va_list).
        template <class T, ENABLE_IF_SUPPORTED_CHARACTER_TYPE(T)>
        static std::unique_ptr<T[ ]> BuildFormattedMessage(const T* pFormat, ...);     

    public:

        /// Dummy Log Methods For Unit Tests (adhere to ILogger interface) \\\

        bool Log(const VerbosityLevel&, const utf8*, ...)
        {
            return false;
        }

        bool Log(const VerbosityLevel&, const utf16*, ...)
        {
            return false;
        }

        bool Log(const VerbosityLevel&, const utf8*, va_list)
        {
            return false;
        }

        bool Log(const VerbosityLevel&, const utf16*, va_list)
        {
            return false;
        }

        bool Log(const VerbosityLevel&, const std::thread::id&, const utf8*, ...)
        {
            return false;
        }

        bool Log(const VerbosityLevel&, const std::thread::id&, const utf16*, ...)
        {
            return false;
        }        

        bool Log(const VerbosityLevel&, const std::thread::id&, const utf8*, va_list)
        {
            return false;
        }

        bool Log(const VerbosityLevel&, const std::thread::id&, const utf16*, va_list)
        {
            return false;
        }
    };
}