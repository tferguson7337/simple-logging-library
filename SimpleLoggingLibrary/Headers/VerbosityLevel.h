#pragma once

namespace SLL
{
    // Various message-type categories.
    enum class VerbosityLevel : size_t
    {
        // Standard log message.
        INFO = 0,

        // Noteworthy, yet expected behavior.
        WARN,

        // Erroneous behavior.
        ERROR,

        // Erroneous behavior leading to unrecoverable state. 
        FATAL,

        // MAX CAP
        MAX
    };
}
