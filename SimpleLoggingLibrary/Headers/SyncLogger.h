#pragma once

// SLL
#include "LoggerBase.h"
#include "ConfigPackage.h"

// STL
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>


namespace SLL
{
    ///
    //
    //  Class   - SyncLogger
    //
    //  Purpose - Provide a thread-safe wrapper around an arbitrary SLL logger object.
    //            Note: SyncLogger includes own worker thread that performs the actual logging.
    //                  
    //
    ///
    class SyncLogger : public virtual LoggerBase, public virtual ILogger
    {
        /// No move.
        SyncLogger(SyncLogger&&) = delete;
        SyncLogger& operator=(SyncLogger&&) = delete;

    private:
        /// Private LogMessage Class \\\

        // Encapsulates minimum required log message data into a single package.
        class LogMessage
        {
            /// No copy.
            LogMessage(const LogMessage&) = delete;
            LogMessage& operator=(const LogMessage&) = delete;

        private:
            VerbosityLevel lvl;
            std::unique_ptr<utf16[ ]> str;
            std::thread::id tid;

            LogMessage( ) noexcept :
                lvl(VerbosityLevel::MAX),
                str(nullptr),
                tid(std::thread::id( ))
            { }

        public:
            LogMessage(const VerbosityLevel& l, std::unique_ptr<utf16[ ]>&& s, const std::thread::id& t) :
                lvl(l),
                str(std::move(s)),
                tid(t)
            { }

            LogMessage(LogMessage&& src) noexcept :
                LogMessage( )
            {
                *this = std::move(src);
            }

            ~LogMessage( ) = default;

            LogMessage& operator=(LogMessage&& src) noexcept
            {
                if ( this != &src )
                {
                    lvl = src.lvl;
                    str = std::move(src.str);
                    tid = std::move(src.tid);

                    src.lvl = VerbosityLevel::MAX;
                    src.tid = std::thread::id( );
                }

                return *this;
            }

            const VerbosityLevel& GetVerbosityLevel( ) const noexcept
            {
                return lvl;
            }

            const utf16* GetString( ) const noexcept
            {
                return str.get( );
            }

            const std::thread::id& GetThreadID( ) const noexcept
            {
                return tid;
            }
        };

        /// Private Data Members \\\

        // Logger Data
        OptionFlag mOptionMask;
        std::shared_ptr<ILogger> mpLogger;
        size_t mSuccessCounter;
        size_t mFailCounter;
        
        // Message Queue
        std::queue<LogMessage> mMsgQueue;
        std::mutex mMsgQueueMutex;
        std::atomic<size_t> mMsgQueueSize;

        // Worker Thread
        std::thread mWorkerThread;
        std::atomic<bool> mTerminate;
        std::condition_variable mMsgCV;

        /// Private Worker Methods \\\

        // Worker Thread Methods
        void WorkerLogLoop( );
        void WaitForMsgs( );
        bool WaitPredicate( ) const noexcept;
        bool TerminatePredicate( );
        void PushMsg(LogMessage&& msg);
        std::queue<LogMessage> GetQueuedMsgs( );
        void LogMsgs(std::queue<LogMessage>&&);

    public:
        /// Constructors \\\

        // Single-ConfigPackage Constructor [C]
        SyncLogger(const ConfigPackage&);

        // Multiple-ConfigPackage Constructor [C]
        SyncLogger(const ConfigPackage&, const ConfigPackage&);

        /// Destructor \\\

        ~SyncLogger( );

        /// Public Methods \\\

        // Submit log message to stream(s) (variadic arguments).
        bool Log(const VerbosityLevel& lvl, const utf8* pFormat, ...);
        bool Log(const VerbosityLevel& lvl, const utf16* pFormat, ...);

        // Submit log message to stream(s) (variadic arguments, explicit thread ID).
        bool Log(const VerbosityLevel& lvl, const std::thread::id& tid, const utf8* pFormat, ...);
        bool Log(const VerbosityLevel& lvl, const std::thread::id& tid, const utf16* pFormat, ...);

        // Submit log message to stream(s) (va_list).
        bool Log(const VerbosityLevel& lvl, const utf8* pFormat, va_list pArgs);
        bool Log(const VerbosityLevel& lvl, const utf16* pFormat, va_list pArgs);

        // Submit log message to stream(s) (va_list, explicit thread ID).
        bool Log(const VerbosityLevel& lvl, const std::thread::id& tid, const utf8* pFormat, va_list pArgs);
        bool Log(const VerbosityLevel& lvl, const std::thread::id& tid, const utf16* pFormat, va_list pArgs);
    };
}