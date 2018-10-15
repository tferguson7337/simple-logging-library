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

    // Color String Getter
    template <class T>
    const std::basic_string<T>& ColorConverter::GetColorString(const size_t i)
    {
        static const std::vector<SupportedStringTuple> colorStrings
        {
            // Basic Colors
            MAKE_STR_TUPLE("BLACK"),
            MAKE_STR_TUPLE("RED"),
            MAKE_STR_TUPLE("GREEN"),
            MAKE_STR_TUPLE("YELLOW"),
            MAKE_STR_TUPLE("BLUE"),
            MAKE_STR_TUPLE("MAGENTA"),
            MAKE_STR_TUPLE("CYAN"),
            MAKE_STR_TUPLE("WHITE"),

            // Bright Colors
            MAKE_STR_TUPLE("BRIGHT_BLACK"),
            MAKE_STR_TUPLE("BRIGHT_RED"),
            MAKE_STR_TUPLE("BRIGHT_GREEN"),
            MAKE_STR_TUPLE("BRIGHT_YELLOW"),
            MAKE_STR_TUPLE("BRIGHT_BLUE"),
            MAKE_STR_TUPLE("BRIGHT_MAGENTA"),
            MAKE_STR_TUPLE("BRIGHT_CYAN"),
            MAKE_STR_TUPLE("BRIGHT_WHITE"),

            // Default
            MAKE_STR_TUPLE("DEFAULT")
        };

        if ( i >= colorStrings.size( ) )
        {
            throw std::range_error(
                __FUNCTION__" - Index argument out of bounds.  Index = " +
                std::to_string(i) +
                ", Max = " +
                std::to_string(colorStrings.size( ) - 1)
            );
        }

        return std::get<std::basic_string<T>>(colorStrings[i]);
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
    template <class T>
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

    template const std::basic_string<utf8>& ColorConverter::ToString<utf8>(Color);
    template const std::basic_string<utf16>& ColorConverter::ToString<utf16>(Color);
}