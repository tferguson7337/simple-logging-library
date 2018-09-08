#pragma once

#include <StreamLogger.h>

namespace SLL
{
    ///
    //
    //  Class   - DualLogger
    //
    //  Purpose - Wrapper for StdOutLogger and FileLogger.
    //
    ///
    class DualLogger : public virtual ILogger
    {
    private:
        /// Private Data Members \\\

        StdOutLogger mStdOutLogger;
        FileLogger mFileLogger;

    public:
        /// Constructors \\\

        // Shared ConfigPackage Constructor [C]
        DualLogger(const ConfigPackage& config);

        // Shared ConfigPackage Constructor [M]
        DualLogger(ConfigPackage&& config);

        // Separate ConfigPackages Constructor [C]
        DualLogger(const ConfigPackage& stdOutConfig, const ConfigPackage& fileConfig);

        // Separate ConfigPackages Constructor [M]
        DualLogger(ConfigPackage&& stdOutConfig, ConfigPackage&& fileConfig);

        // Move Constructor
        DualLogger(DualLogger&&);

        /// Destructor \\\

        ~DualLogger( ) noexcept;

        /// Assignment Overload \\\

        // Move Assignment
        DualLogger& operator=(DualLogger&&);

        /// Getters \\\

        // Get immutable ConfigPackage for specified LoggerType
        template <class LoggerType>
        const ConfigPackage& GetConfig( ) const noexcept;

        /// Public Methods \\\

        // Submit log message to stream(s) (variadic arguments).
        bool Log(const VerbosityLevel&, const char*, ...);
        bool Log(const VerbosityLevel&, const wchar_t*, ...);

        // Submit log message to stream(s) (variadic arguments, explicit thread ID).
        bool Log(const VerbosityLevel&, const std::thread::id&, const char*, ...);
        bool Log(const VerbosityLevel&, const std::thread::id&, const wchar_t*, ...);

        // Submit log message to stream(s) (va_list).
        bool Log(const VerbosityLevel&, const char*, va_list);
        bool Log(const VerbosityLevel&, const wchar_t*, va_list);

        // Submit log message to stream(s) (va_list, explicit thread ID).
        bool Log(const VerbosityLevel&, const std::thread::id&, const char*, va_list);
        bool Log(const VerbosityLevel&, const std::thread::id&, const wchar_t*, va_list);
    };
}