#include <DualLogger.h>

#include <thread>

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

     // Submit log message to stream(s) (variadic arguments, narrow).
    bool DualLogger::Log(const VerbosityLevel& lvl, const char* pFormat, ...)
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

    // Submit log message to stream(s) (variadic arguments, wide).
    bool DualLogger::Log(const VerbosityLevel& lvl, const wchar_t* pFormat, ...)
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

    // Submit log message to stream(s) (variadic arguments, explicit thread ID, narrow).
    bool DualLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const char* pFormat, ...)
    {
        bool ret = false;

        va_list pArgs;
        va_start(pArgs, pFormat);

        try
        {
            ret = Log(lvl, tid, pFormat, pArgs);
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

    // Submit log message to stream(s) (variadic arguments, explicit thread ID, wide).
    bool DualLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const wchar_t* pFormat, ...)
    {
        bool ret = false;

        va_list pArgs;
        va_start(pArgs, pFormat);

        try
        {
            ret = Log(lvl, tid, pFormat, pArgs);
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

    // Submit log message to stream(s) (va_list, narrow).
    bool DualLogger::Log(const VerbosityLevel& lvl, const char* pFormat, va_list pArgs)
    {
        return Log(lvl, std::this_thread::get_id( ), pFormat, pArgs);
    }

    // Submit log message to stream(s) (va_list, wide).
    bool DualLogger::Log(const VerbosityLevel& lvl, const wchar_t* pFormat, va_list pArgs)
    {
        return Log(lvl, std::this_thread::get_id( ), pFormat, pArgs);
    }

    // Submit log message to stream(s) (va_list, explicit thread ID, narrow).
    bool DualLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const char* pFormat, va_list pArgs)
    {
        // Both StreamLogger objects handle sanity checks and errors.
        return mStdOutLogger.Log(lvl, tid, pFormat, pArgs) && mFileLogger.Log(lvl, tid, pFormat, pArgs);
    }

    // Submit log message to stream(s) (va_list, explicit thread ID, wide).
    bool DualLogger::Log(const VerbosityLevel& lvl, const std::thread::id& tid, const wchar_t* pFormat, va_list pArgs)
    {
        // Both StreamLogger objects handle sanity checks and errors.
        return mStdOutLogger.Log(lvl, tid, pFormat, pArgs) && mFileLogger.Log(lvl, tid, pFormat, pArgs);
    }
}