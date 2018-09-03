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

        // Log [va_list]
        template <class T, ENABLE_IF_SUPPORTED_CHARACTER_TYPE(T)>
        bool Log(const VerbosityLevel& lvl, const T* pFormat, va_list pArgs);

        // Log [variadic arguments]
        template <class T, ENABLE_IF_SUPPORTED_CHARACTER_TYPE(T)>
        bool Log(const VerbosityLevel& lvl, const T* pFormat, ...);
    };
}