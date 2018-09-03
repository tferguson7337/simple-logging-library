#include <DualLogger.h>

namespace SLL
{
    /// Constructors \\\

    // Shared ConfigPackage Constructor [C] 
    DualLogger::DualLogger(const ConfigPackage& config) :
        mStdOutLogger(config),
        mFileLogger(config)
    { }

    // Shared ConfigPackage Constructor [M]
    // - Note: Can't move both times, so copy first time then move second time.
    DualLogger::DualLogger(ConfigPackage&& config) :
        mStdOutLogger(config),
        mFileLogger(std::move(config))
    { }

    // Separate ConfigPackages Constructor [C]
    DualLogger::DualLogger(const ConfigPackage& stdOutConfig, const ConfigPackage& fileConfig) :
        mStdOutLogger(stdOutConfig),
        mFileLogger(fileConfig)
    { }

    // Separate ConfigPackages Constructor [M]
    DualLogger::DualLogger(ConfigPackage&& stdOutConfig, ConfigPackage&& fileConfig) :
        mStdOutLogger(std::forward<ConfigPackage>(stdOutConfig)),
        mFileLogger(std::forward<ConfigPackage>(fileConfig))
    { }

    // Move Constructor
    DualLogger::DualLogger(DualLogger&& src) :
        mStdOutLogger(std::forward<StdOutLogger>(src.mStdOutLogger)),
        mFileLogger(std::forward<FileLogger>(src.mFileLogger))
    { }

    /// Destructor \\\

    // Default Destructor
    DualLogger::~DualLogger( ) noexcept = default;

    /// Assignment Operator \\\

    // Move Assignment
    DualLogger& DualLogger::operator=(DualLogger&& src)
    {
        if ( this == &src )
        {
            throw std::invalid_argument(__FUNCTION__" - Attempted self-assignment.");
        }

        mStdOutLogger = std::forward<StdOutLogger>(src.mStdOutLogger);
        mFileLogger = std::forward<FileLogger>(src.mFileLogger);

        return *this;
    }

    /// Getters \\\

    // Get immutable ConfigPackage reference for FileLogger
    template <>
    const ConfigPackage& DualLogger::GetConfig<FileLogger>( ) const noexcept
    {
        return mFileLogger.GetConfig( );
    }

    // Get immutable ConfigPackage reference for StdOutLogger
    template <>
    const ConfigPackage& DualLogger::GetConfig<StdOutLogger>( ) const noexcept
    {
        return mStdOutLogger.GetConfig( );
    }

    /// Public Methods \\\

    template <class T, typename>
    bool DualLogger::Log(const VerbosityLevel& lvl, const T* pFormat, va_list pArgs)
    {    
        // Both StreamLogger objects handle sanity checks and errors.
        return mStdOutLogger.Log(lvl, pFormat, pArgs) && mFileLogger.Log(lvl, pFormat, pArgs);
    }

    template <class T, typename>
    bool DualLogger::Log(const VerbosityLevel& lvl, const T* pFormat, ...)
    {
        bool ret = false;

        va_list pArgs;
        va_start(pArgs, pFormat);

        try
        {
            ret = Log(lvl, pFormat, pArgs);
        }
        catch ( const std::exception& )
        {
            // Cleanup and pass the exception further down the call stack.
            va_end(pArgs);
            throw;
        }

        va_end(pArgs);

        return ret;
    }
}