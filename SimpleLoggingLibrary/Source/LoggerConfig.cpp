// Class Declaration
#include <LoggerConfig.h>

// Enum Class Definitions
#include <LogColor.h>
#include <LoggerOption.h>
#include <VerbosityLevel.h>

/// Explicit Template Instantiations \\\

template class SLL::LoggerConfig<char>;
template class SLL::LoggerConfig<wchar_t>;

namespace SLL
{
    /// PRIVATE HELPERS \\\

    // Private Helper - Validate VerbosityLevel
    template <class T>
    void LoggerConfig<T>::ValidateVerbosityLevel(const VerbosityLevel lvl, const std::string& f) const
    {
        if ( lvl < VerbosityLevel::INFO || lvl >= VerbosityLevel::MAX )
        {
            throw std::invalid_argument(f + " - Invalid verbosity level (" + std::to_string(static_cast<std::underlying_type_t<VerbosityLevel>>(lvl)) + ").");
        }
    }

    // Private Helper - Validate LogColor
    template <class T>
    void LoggerConfig<T>::ValidateLogColor(const LogColor color, const std::string& f) const
    {
        if ( color < LogColor::WHITE || color >= LogColor::MAX )
        {
            throw std::invalid_argument(f + " - Invalid log color (" + std::to_string(static_cast<std::underlying_type_t<LogColor>>(color)) + ").");
        }
    }

    // Private Helper - Validate LoggerOption
    template <class T>
    void LoggerConfig<T>::ValidateLoggerOption(const LoggerOption opt, const std::string& f) const
    {
        if ( opt < LoggerOption::LogToStdout || opt >= LoggerOption::MAX )
        {
            throw std::invalid_argument(f + " - Invalid logger option (" + std::to_string(static_cast<std::underlying_type_t<LoggerOption>>(opt)) + ").");
        }
    }

    /// CTORS \\\

    // Default Ctor
    template <class T>
    LoggerConfig<T>::LoggerConfig( ) :
        mVerbosityColors(static_cast<size_t>(VerbosityLevel::MAX), LogColor::WHITE),
        mOptionMask(LoggerOption::NONE),
        mVerbosityThreshold(VerbosityLevel::INFO)
    {

    }

    // Copy Ctor
    template <class T>
    LoggerConfig<T>::LoggerConfig(const LoggerConfig& src) :
        LoggerConfig( )
    {
        *this = src;
    }

    // Move Ctor
    template <class T>
    LoggerConfig<T>::LoggerConfig(LoggerConfig&& src) noexcept :
        LoggerConfig( )
    {
        *this = std::move(src);
    }

    /// ASSIGNMENT OVERLOADS \\\

    // Copy Assignment
    template <class T>
    LoggerConfig<T>& LoggerConfig<T>::operator=(const LoggerConfig& src)
    {
        mLogFile            = src.mLogFile;
        mVerbosityColors    = src.mVerbosityColors;
        mOptionMask         = src.mOptionMask;
        mVerbosityThreshold = src.mVerbosityThreshold;

        return *this;
    }

    // Move Assignment
    template <class T>
    LoggerConfig<T>& LoggerConfig<T>::operator=(LoggerConfig&& src)
    {
        mLogFile            = std::move(src.mLogFile);
        mVerbosityColors    = std::move(src.mVerbosityColors);
        mOptionMask         = src.mOptionMask;
        mVerbosityThreshold = src.mVerbosityThreshold;

        return *this;
    }

    /// GETTERS \\\

    // Getter - Log Color for VerbosityLevel
    template <class T>
    LogColor LoggerConfig<T>::GetLogColor(const VerbosityLevel lvl) const
    {
        ValidateVerbosityLevel(lvl, __FUNCTION__);

        return mVerbosityColors[static_cast<size_t>(lvl)];
    }

    // Getter - VerbosityThreshold Setting
    template <class T>
    VerbosityLevel LoggerConfig<T>::GetVerbosityThreshold( ) const noexcept
    {
        return mVerbosityThreshold;
    }

    // Getter - Target Log File
    template <class T>
    const std::basic_string<T>& LoggerConfig<T>::GetFile( ) const noexcept
    {
        return mLogFile;
    }

    /// SETTERS \\\

    // Setter - Log Color for VerbosityLevel
    template <class T>
    void LoggerConfig<T>::SetLogColor(const LogColor color, const VerbosityLevel lvl)
    {
        ValidateLogColor(color, __FUNCTION__);
        ValidateVerbosityLevel(lvl, __FUNCTION__);

        mVerbosityColors[static_cast<size_t>(lvl)] = color;
    }

    // Setter - Set Minimum VerbosityThreshold
    template <class T>
    void LoggerConfig<T>::SetVerbosityThreshold(const VerbosityLevel lvl)
    {
        ValidateVerbosityLevel(lvl, __FUNCTION__);

        mVerbosityThreshold = lvl;
    }

    // Setter (Copy) - Set Target Log File
    template <class T>
    void LoggerConfig<T>::SetFile(const std::basic_string<T>& file)
    {
        mLogFile = file;
    }

    // Setter (Move) - Set Target Log File
    template <class T>
    void LoggerConfig<T>::SetFile(std::basic_string<T>&& file) noexcept
    {
        mLogFile = std::move(file);
    }

    /// PUBLIC METHODS \\\

    // Public Method - Enable LoggerOption
    // If opt contains more than one option, all are enabled.
    template <class T>
    void LoggerConfig<T>::Enable(const LoggerOption opt)
    {
        ValidateLoggerOption(opt, __FUNCTION__);

        mOptionMask |= opt;
    }

    // Public Method - Disable LoggerOption
    // If opt contains more than one option, all are disabled.
    template <class T>
    void LoggerConfig<T>::Disable(const LoggerOption opt)
    {
        ValidateLoggerOption(opt, __FUNCTION__);

        mOptionMask &= ~opt;
    }

    // Public Method - Check if LoggerOption is Enabled
    // If opt contains more than one option, this will return
    // true only if all options are enabled.
    template <class T>
    bool LoggerConfig<T>::OptionEnabled(const LoggerOption opt) const
    {
        ValidateLoggerOption(opt, __FUNCTION__);

        return (mOptionMask & opt) == opt;
    }

}