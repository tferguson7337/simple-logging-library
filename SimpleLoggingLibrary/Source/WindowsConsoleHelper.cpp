// For enabling Virtual Terminal
#include <Windows.h>

// Synchronization.
#include <atomic>
#include <mutex>

// Class Declaration
#include <WindowsConsoleHelper.h>

namespace SLL
{
    /// Static Private Helper Methods \\\

    // Enable Windows virtual terminal processing so we can log in color.
    // Note:  Only supported on Windows 10, Threshold 2 (Nov 2015) and beyond.
    bool WindowsConsoleHelper::EnableVirtualTerminal( )
    {
        // Set output mode to handle virtual terminal sequences
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if ( !hStdOut || hStdOut == INVALID_HANDLE_VALUE )
        {
            return false;
        }

        // Get current console mode settings.
        DWORD dwMode = 0;
        if ( !GetConsoleMode(hStdOut, &dwMode) )
        {
            return false;
        }

        // Add virtual terminal processing to current console mode.
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if ( !SetConsoleMode(hStdOut, dwMode) )
        {
            return false;
        }

        return true;
    }

    /// Static Public Method \\\

    // Thread-safe initialization to enable virtual terminal processing.
    bool WindowsConsoleHelper::Initialize( )
    {
        static std::atomic<bool> init = false;
        static std::mutex initLock;

        // See if we already init'd.
        if ( init )
        {
            return true;
        }

        // Protect against parallel init attempts.
        std::lock_guard<std::mutex> lg(initLock);

        // After the first thread finishes, other threads that may have been waiting on the lock
        // will reach this point.  Avoid re-initing and just exit early if it's already successfully happened.
        if ( init )
        {
            return true;
        }

        // Attempt to enable virtual terminal processing.
        init = EnableVirtualTerminal( );

        return init;
    }


}