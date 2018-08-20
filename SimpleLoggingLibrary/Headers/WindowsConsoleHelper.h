#pragma once

// For SLL::Color.
#include <Color.h>

namespace SLL
{
    class WindowsConsoleHelper
    {
        // Fully static class - no instances allowed.
        WindowsConsoleHelper( ) = delete;
        WindowsConsoleHelper(const WindowsConsoleHelper&) = delete;
        WindowsConsoleHelper(WindowsConsoleHelper&&) = delete;

        ~WindowsConsoleHelper( ) = delete;

        // No instances = no copy or move assignment.
        WindowsConsoleHelper& operator=(const WindowsConsoleHelper&) = delete;
        WindowsConsoleHelper& operator=(WindowsConsoleHelper&&) = delete;

    private:
        /// Static Private Helper Methods \\\

        // Enables Virtual Console processing so cmd can interpret color sequences.
        static bool EnableVirtualTerminal( );

    public:
        /// Static Public Method \\\

        // Initialize to get console's original foreground color prior to logging.
        static bool Initialize( ); 
    };
}
