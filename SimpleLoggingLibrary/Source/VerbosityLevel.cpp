#include <VerbosityLevel.h>

#include <stdexcept>
#include <vector>

namespace SLL
{
    /// Private Helper Methods \\\

    // VerbosityLevel String Getter
    template <class T, typename>
    const std::basic_string<T>& VerbosityLevelConverter::GetVerbosityLevelString(const size_t i)
    {
        static const std::vector<SupportedStringTuple> VerbosityLevelStrings
        {
            MAKE_STR_TUPLE("INFO"),
            MAKE_STR_TUPLE("WARN"),
            MAKE_STR_TUPLE("ERROR"),
            MAKE_STR_TUPLE("FATAL")
        };

        if ( i >= VerbosityLevelStrings.size( ) )
        {
            throw std::range_error(
                __FUNCTION__" - Index argument out of bounds.  Index = " +
                std::to_string(i) +
                ", Max = " +
                std::to_string(VerbosityLevelStrings.size( ) - 1)
            );
        }

        if constexpr ( !IsSupportedCharType<T>( ) )
        {
            throw std::runtime_error(__FUNCTION__": Unsupported template type.");
        }

        return std::get<std::basic_string<T>>(VerbosityLevelStrings[i]);
    }

    /// Public Methods \\\

    // Returns underlying scalar type value of VerbosityLevel argument.
    VerbosityLevelType VerbosityLevelConverter::ToScalar(VerbosityLevel lvl)
    {
        if ( lvl < VerbosityLevel::BEGIN || lvl > VerbosityLevel::MAX )
        {
            throw std::invalid_argument(
                __FUNCTION__" - Invalid verbosity level argument (" +
                std::to_string(static_cast<VerbosityLevelType>(lvl)) +
                ")."
            );
        }

        return static_cast<VerbosityLevelType>(lvl);
    }

    // Returns string name of VerbosityLevel argument.
    template <class T, typename>
    const std::basic_string<T>& VerbosityLevelConverter::ToString(VerbosityLevel lvl)
    {
        if ( lvl >= VerbosityLevel::MAX )
        {
            throw std::invalid_argument(
                __FUNCTION__" - Invalid verbosity level argument (" +
                std::to_string(static_cast<VerbosityLevelType>(lvl)) +
                ")."
            );
        }

        return GetVerbosityLevelString<T>(ToScalar(lvl));
    }


    /// Explicit Template Instantiation \\\

    template const std::basic_string<utf8>& VerbosityLevelConverter::ToString<utf8>(VerbosityLevel);
    template const std::basic_string<utf16>& VerbosityLevelConverter::ToString<utf16>(VerbosityLevel);
}
