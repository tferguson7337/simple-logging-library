#include <Color.h>

#include <vector>

namespace SLL
{
    /// OptionFlag Enum Class Operator Overloads \\\

    // Bitwise NOT
    Color operator~(const Color rhs)
    {
        return static_cast<Color>(~static_cast<ColorType>(rhs) & (static_cast<ColorType>(Color::BASIC_END) - 1));
    }

    // Bitwise OR
    Color operator|(const Color lhs, const Color rhs)
    {
        return static_cast<Color>(static_cast<ColorType>(lhs) | static_cast<ColorType>(rhs));
    }

    // Bitwise AND
    Color operator&(const Color lhs, const Color rhs)
    {
        return static_cast<Color>(static_cast<ColorType>(lhs) & static_cast<ColorType>(rhs));
    }

    // Bitwise XOR
    Color operator^(const Color lhs, const Color rhs)
    {
        return static_cast<Color>(static_cast<ColorType>(lhs) ^ static_cast<ColorType>(rhs));
    }


    // Bitwise OR - Assignment
    Color operator|=(Color& lhs, const Color rhs)
    {
        return lhs = lhs | rhs;
    }

    // Bitwise AND - Assignment
    Color operator&=(Color& lhs, const Color rhs)
    {
        return lhs = lhs & rhs;
    }

    // Bitwise XOR - Assignment
    Color operator^=(Color& lhs, const Color rhs)
    {
        return lhs = lhs ^ rhs;
    }


    // Left Shift
    Color operator<<(const Color lhs, const ColorType s)
    {
        ColorType val = static_cast<ColorType>(lhs) << s;
        return (val > static_cast<ColorType>(Color::BASIC_END)) ? Color::BASIC_END : static_cast<Color>(val);
    }

    // Right Shift
    Color operator>>(const Color lhs, const ColorType s)
    {
        return static_cast<Color>(static_cast<ColorType>(lhs) >> s);
    }

    // Left Shift - Assignment
    Color operator<<=(Color& lhs, const ColorType s)
    {
        return lhs = lhs << s;
    }

    // Right Shift - Assignment
    Color operator>>=(Color& lhs, const ColorType s)
    {
        return lhs = lhs >> s;
    }


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

            // Bright Colors
            "BRIGHT_BLACK",
            "BRIGHT_RED",
            "BRIGHT_GREEN",
            "BRIGHT_YELLOW",
            "BRIGHT_BLUE",
            "BRIGHT_MAGENTA",
            "BRIGHT_CYAN",
            "BRIGHT_WHITE",

            // Default
            "DEFAULT"
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

            // Bright Colors
            L"BRIGHT_BLACK",
            L"BRIGHT_RED",
            L"BRIGHT_GREEN",
            L"BRIGHT_YELLOW",
            L"BRIGHT_BLUE",
            L"BRIGHT_MAGENTA",
            L"BRIGHT_CYAN",
            L"BRIGHT_WHITE",

            // Default
            L"DEFAULT"
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