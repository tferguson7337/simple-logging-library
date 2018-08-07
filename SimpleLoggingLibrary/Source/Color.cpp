#include <Color.h>

#include <vector>

namespace SLL
{
    /// Private Helper Methods \\\

    // Color String Getter - char
    template <>
    const std::basic_string<char>& ColorConverter::GetColorString<char>(const size_t i)
    {
        static const std::vector<std::basic_string<char>> colorStringsA
        {
            // Basic Colors
            "BLACK",
            "RED",
            "GREEN",
            "YELLOW",
            "BLUE",
            "MAGENTA",
            "CYAN",
            "WHITE",
        };

        if ( i >= colorStringsA.size( ) )
        {
            throw std::range_error(
                __FUNCTION__" - Index argument out of bounds.  Index = " +
                std::to_string(i) +
                ", Max = " +
                std::to_string(colorStringsA.size( ) - 1)
            );
        }

        return colorStringsA[i];
    }

    // Color String Getter - wchar_t
    template <>
    const std::basic_string<wchar_t>& ColorConverter::GetColorString<wchar_t>(const size_t i)
    {
        static const std::vector<std::basic_string<wchar_t>> colorStringsW
        {
            // Basic Colors
            L"BLACK",
            L"RED",
            L"GREEN",
            L"YELLOW",
            L"BLUE",
            L"MAGENTA",
            L"CYAN",
            L"WHITE",
        };

        if ( i >= colorStringsW.size( ) )
        {
            throw std::range_error(
                __FUNCTION__" - Index argument out of bounds.  Index = " +
                std::to_string(i) +
                ", Max = " +
                std::to_string(colorStringsW.size( ) - 1)
            );
        }

        return colorStringsW[i];
    }


    /// Public Methods \\\

    // Returns underlying scalar type value of Color argument.
    ColorType ColorConverter::ToScalar(Color c)
    {
        if ( c > Color::MAX )
        {
            throw std::invalid_argument(
                __FUNCTION__" - Invalid color argument (" +
                std::to_string(static_cast<ColorType>(c)) +
                ")."
            );
        }

        return static_cast<ColorType>(c);
    }

    // Returns string name of Color argument.
    template <class T, typename>
    const std::basic_string<T>& ColorConverter::ToString(Color c)
    {
        if ( c >= Color::MAX )
        {
            throw std::invalid_argument(
                __FUNCTION__" - Invalid color argument (" +
                std::to_string(static_cast<ColorType>(c)) +
                ")."
            );
        }

        return GetColorString<T>(ToScalar(c));
    }


    /// Explicit Template Instantiation \\\

    template const std::basic_string<char>& ColorConverter::ToString<char>(Color);
    template const std::basic_string<wchar_t>& ColorConverter::ToString<wchar_t>(Color);
}