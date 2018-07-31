#include <Color.h>

#include <vector>

/// Private Helper Methods \\\

// Color String Getter - char
template <>
const std::basic_string<char>& SLL::ColorConverter::GetColorString<char>(const size_t i)
{
    static const std::vector<std::basic_string<char>> colorStringsA
    {
        // Basic Shades
        "WHITE",
        "LIGHT_GREY",
        "GREY",
        "DARK_GREY",
        "BLACK",

        // Basic Colors
        "DARK_RED",
        "RED",
        "ORANGE",
        "YELLOW",
        "YELLOW_GREEN",
        "GREEN",
        "CYAN",
        "BLUE",
        "VIOLET",
        "PURPLE"
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
const std::basic_string<wchar_t>& SLL::ColorConverter::GetColorString<wchar_t>(const size_t i)
{
    static const std::vector<std::basic_string<wchar_t>> colorStringsW
    {
        // Basic Shades
        L"WHITE",
        L"LIGHT_GREY",
        L"GREY",
        L"DARK_GREY",
        L"BLACK",

        // Basic Colors
        L"DARK_RED",
        L"RED",
        L"ORANGE",
        L"YELLOW",
        L"YELLOW_GREEN",
        L"GREEN",
        L"CYAN",
        L"BLUE",
        L"VIOLET",
        L"PURPLE"
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
SLL::ColorType SLL::ColorConverter::ToScalar(Color c)
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
const std::basic_string<T>& SLL::ColorConverter::ToString(Color c)
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

template const std::basic_string<char>& SLL::ColorConverter::ToString<char>(Color);
template const std::basic_string<wchar_t>& SLL::ColorConverter::ToString<wchar_t>(Color);