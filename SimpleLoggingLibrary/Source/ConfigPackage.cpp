// Class Header and Other Includes
#include <ConfigPackage.h>

namespace SLL
{
    /// PRIVATE HELPERS \\\

    // Private Helper - Validate VerbosityLevel
    void ConfigPackage::ValidateVerbosityLevel(const VerbosityLevel lvl, const std::string& f) const
    {
        if ( lvl < VerbosityLevel::BEGIN || lvl >= VerbosityLevel::MAX )
        {
            throw std::invalid_argument(f + " - Invalid verbosity level (" + std::to_string(static_cast<std::underlying_type_t<VerbosityLevel>>(lvl)) + ").");
        }
    }

    // Private Helper - Validate Color
    void ConfigPackage::ValidateColor(const Color color, const std::string& f) const
    {
        if ( color < Color::BEGIN || color >= Color::MAX )
        {
            throw std::invalid_argument(f + " - Invalid log color (" + std::to_string(static_cast<std::underlying_type_t<Color>>(color)) + ").");
        }
    }

    // Private Helper - Validate OptionFlag
    void ConfigPackage::ValidateOptionFlag(const OptionFlag opt, const std::string& f) const
    {
        if ( opt < OptionFlag::BEGIN || opt >= OptionFlag::MAX )
        {
            throw std::invalid_argument(f + " - Invalid logger option (" + std::to_string(static_cast<std::underlying_type_t<OptionFlag>>(opt)) + ").");
        }
    }

    /// CTORS \\\

    // Default Ctor
    ConfigPackage::ConfigPackage( ) :
        mVerbosityColors(static_cast<size_t>(VerbosityLevel::MAX), Color::WHITE),
        mOptionMask(OptionFlag::NONE),
        mVerbosityThreshold(VerbosityLevel::INFO)
    { }

    // Copy Ctor
    ConfigPackage::ConfigPackage(const ConfigPackage& src) :
        ConfigPackage( )
    {
        *this = src;
    }

    // Move Ctor
    ConfigPackage::ConfigPackage(ConfigPackage&& src) noexcept :
        ConfigPackage( )
    {
        *this = std::move(src);
    }

    /// ASSIGNMENT OVERLOADS \\\

    // Copy Assignment
    ConfigPackage& ConfigPackage::operator=(const ConfigPackage& src)
    {
        mLogFile = src.mLogFile;
        mVerbosityColors = src.mVerbosityColors;
        mOptionMask = src.mOptionMask;
        mVerbosityThreshold = src.mVerbosityThreshold;

        return *this;
    }

    // Move Assignment
    ConfigPackage& ConfigPackage::operator=(ConfigPackage&& src)
    {
        mLogFile = std::move(src.mLogFile);
        mVerbosityColors = std::move(src.mVerbosityColors);
        mOptionMask = src.mOptionMask;
        mVerbosityThreshold = src.mVerbosityThreshold;

        return *this;
    }

    /// GETTERS \\\

    // Getter - Log Color for VerbosityLevel
    Color ConfigPackage::GetColor(const VerbosityLevel lvl) const
    {
        ValidateVerbosityLevel(lvl, __FUNCTION__);

        return mVerbosityColors[static_cast<size_t>(lvl)];
    }

    // Getter - VerbosityThreshold Setting
    VerbosityLevel ConfigPackage::GetVerbosityThreshold( ) const noexcept
    {
        return mVerbosityThreshold;
    }

    // Getter - Target Log File
    const std::wstring& ConfigPackage::GetFile( ) const noexcept
    {
        return mLogFile;
    }

    /// SETTERS \\\

    // Setter - Log Color for VerbosityLevel
    void ConfigPackage::SetColor(const Color color, const VerbosityLevel lvl)
    {
        ValidateColor(color, __FUNCTION__);
        ValidateVerbosityLevel(lvl, __FUNCTION__);

        mVerbosityColors[static_cast<size_t>(lvl)] = color;
    }

    // Setter - Set Minimum VerbosityThreshold
    void ConfigPackage::SetVerbosityThreshold(const VerbosityLevel lvl)
    {
        ValidateVerbosityLevel(lvl, __FUNCTION__);

        mVerbosityThreshold = lvl;
    }

    /// PUBLIC METHODS \\\

    // Public Method - Enable OptionFlag
    // If opt contains more than one option, all are enabled.
    void ConfigPackage::Enable(const OptionFlag opt)
    {
        ValidateOptionFlag(opt, __FUNCTION__);

        mOptionMask |= opt;
    }

    // Public Method - Disable OptionFlag
    // If opt contains more than one option, all are disabled.
    void ConfigPackage::Disable(const OptionFlag opt)
    {
        ValidateOptionFlag(opt, __FUNCTION__);

        mOptionMask &= ~opt;
    }

    // Public Method - Check if OptionFlag is Enabled
    // If opt contains more than one option, this will return
    // true only if all options are enabled.
    bool ConfigPackage::OptionEnabled(const OptionFlag opt) const
    {
        ValidateOptionFlag(opt, __FUNCTION__);

        return (mOptionMask & opt) == opt;
    }
}