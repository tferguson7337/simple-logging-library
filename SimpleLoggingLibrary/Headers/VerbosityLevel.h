#pragma once

#include <Macros.h>

#include <string>

namespace SLL
{
    // Various message-type categories.
    enum class VerbosityLevel : size_t
    {
        // Standard log message.
        INFO = 0,

        // Noteworthy, potentially erroneous behavior.
        WARN,

        // Erroneous behavior.
        ERROR,

        // Erroneous behavior leading to unrecoverable state. 
        FATAL,

        // MAX CAP
        MAX,

        // MIN CAP
        BEGIN = 0
    };

    using VerbosityLevelType = std::underlying_type_t<VerbosityLevel>;

    class VerbosityLevelConverter
    {
        /// Static Class - No Ctors, Dtor, or Assignment Allowed
        VerbosityLevelConverter( ) = delete;
        VerbosityLevelConverter(const VerbosityLevelConverter&) = delete;
        VerbosityLevelConverter(VerbosityLevelConverter&&) = delete;

        ~VerbosityLevelConverter( ) = delete;

        VerbosityLevelConverter& operator=(const VerbosityLevelConverter&) = delete;
        VerbosityLevelConverter& operator=(VerbosityLevelConverter&&) = delete;

    private:

        /// Private Helper Methods \\\

        // VerbosityLevel String Getters
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static const std::basic_string<T>& GetVerbosityLevelString(const size_t);

    public:
        
        // Returns underlying scalar type value of VerbosityLevel argument.
        static VerbosityLevelType ToScalar(VerbosityLevel);

        // Returns string name of VerbosityLevel argument.
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static const std::basic_string<T>& ToString(VerbosityLevel);
    };
}
