// Class Header and Other Includes
#include <ConfigPackage.h>

namespace SLL
{
    /// PRIVATE HELPERS \\\

    // Private Helper - Validate VerbosityLevel
    void ConfigPackage::ValidateVerbosityLevel(const VerbosityLevel lvl, const std::string& f)
    {
        if ( lvl < VerbosityLevel::BEGIN || lvl >= VerbosityLevel::MAX )
        {
            throw std::invalid_argument(f + " - Invalid verbosity level (" + std::to_string(static_cast<VerbosityLevelType>(lvl)) + ").");
        }
    }

    // Private Helper - Validate Color
    void ConfigPackage::ValidateColor(const Color color, const std::string& f)
    {
        if ( color < Color::BASIC_BEGIN || color >= Color::MAX )
        {
            throw std::invalid_argument(f + " - Invalid log color (" + std::to_string(static_cast<ColorType>(color)) + ").");
        }
    }

    // Private Helper - Validate OptionFlag
    void ConfigPackage::ValidateOptionFlag(const OptionFlag opt, const std::string& f)
    {
        if ( opt < OptionFlag::BEGIN || opt >= OptionFlag::MAX )
        {
            throw std::invalid_argument(f + " - Invalid logger option (" + std::to_string(static_cast<OptionFlagType>(opt)) + ").");
        }
    }

    /// CTORS \\\

    // Default Ctor
    ConfigPackage::ConfigPackage( ) :
        mVerbosityColors(static_cast<size_t>(VerbosityLevel::MAX), Color::DEFAULT),
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
        if ( this != &src )
        {
            mLogFile            = src.mLogFile;
            mVerbosityColors    = src.mVerbosityColors;
            mOptionMask         = src.mOptionMask;
            mVerbosityThreshold = src.mVerbosityThreshold;
        }

        return *this;
    }

    // Move Assignment
    ConfigPackage& ConfigPackage::operator=(ConfigPackage&& src) noexcept
    {
        if ( this != &src )
        {
            mLogFile            = std::move(src.mLogFile);
            mVerbosityColors    = std::move(src.mVerbosityColors);
            mOptionMask         = src.mOptionMask;
            mVerbosityThreshold = src.mVerbosityThreshold;
        }

        return *this;
    }

    /// COMPARISON OVERLOADS \\\

    // Equivalency
    bool ConfigPackage::operator==(const ConfigPackage& other) const noexcept
    {
        // Compare option masks.
        if ( mOptionMask != other.mOptionMask )
        {
            return false;
        }

        // Compare verbosity thresholds.
        if ( mVerbosityThreshold != other.mVerbosityThreshold )
        {
            return false;
        }

        // Compare verbosity-color vectors.
        if ( mVerbosityColors.size( ) != other.mVerbosityColors.size( ) )
        {
            return false;
        }
        else
        {
            for ( size_t i = 0; i < mVerbosityColors.size( ); i++ )
            {
                if ( mVerbosityColors[i] != other.mVerbosityColors[i] )
                {
                    return false;
                }
            }
        }

        // Compare target logfiles.
        if ( mLogFile != other.mLogFile )
        {
            return false;
        }

        // All data members matched.
        return true;
    }

    // Non-equivalency
    bool ConfigPackage::operator!=(const ConfigPackage& other) const noexcept
    {
        return !(*this == other);
    }

    /// GETTERS \\\

    // Getter - Log Color for VerbosityLevel
    Color ConfigPackage::GetColor(const VerbosityLevel lvl) const
    {
        ValidateVerbosityLevel(lvl, __FUNCTION__);

        return mVerbosityColors[static_cast<size_t>(lvl)];
    }

    OptionFlag ConfigPackage::GetOptionFlags( ) const noexcept
    {
        return mOptionMask;
    }

    // Getter - VerbosityThreshold Setting
    VerbosityLevel ConfigPackage::GetVerbosityThreshold( ) const noexcept
    {
        return mVerbosityThreshold;
    }

    // Getter - Target Log Filename
    const std::filesystem::path& ConfigPackage::GetFile( ) const noexcept
    {
        return mLogFile;
    }

    /// SETTERS \\\

    // Setter - Log Color for VerbosityLevel
    void ConfigPackage::SetColor(const VerbosityLevel lvl, const Color color)
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

    // Setter [C] - Set Target Log Filename
    void ConfigPackage::SetFile(const std::filesystem::path& file)
    {
        mLogFile = file;
    }

    // Setter [M] - Set Target Log Filename
    void ConfigPackage::SetFile(std::filesystem::path&& file)
    {
        mLogFile = std::move(file);
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

    // Wrapper For OptionsEnabledAll
    bool ConfigPackage::OptionEnabled(const OptionFlag opt) const
    {
        return OptionsEnabledAll(opt);
    }

    // Returns true if specified logger functionality is enabled.
    // If argument specifies more than one option, this
    // will only return true if one or more options are enabled.
    bool ConfigPackage::OptionsEnabledAny(const OptionFlag opts) const
    {
        ValidateOptionFlag(opts, __FUNCTION__);

        return (mOptionMask & opts) != OptionFlag::NONE;
    }

    // Public Method - Check if OptionFlag is Enabled
    // If opt contains more than one option, this will return
    // true only if all options are enabled.
    bool ConfigPackage::OptionsEnabledAll(const OptionFlag opts) const
    {
        ValidateOptionFlag(opts, __FUNCTION__);

        return (mOptionMask & opts) == opts;
    }
}