#pragma once

// SLL
#include <LoggerBase.h>
#include <ConfigPackage.h>

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
    class SyncLogger : public LoggerBase
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
            std::unique_ptr<wchar_t[ ]> str;

            LogMessage( ) noexcept :
                lvl(VerbosityLevel::MAX),
                str(nullptr)
            { }

        public:
            LogMessage(const VerbosityLevel& l, std::unique_ptr<wchar_t[ ]>&& s) :
                lvl(l),
                str(std::move(s))
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

                    src.lvl = VerbosityLevel::MAX;
                }

                return *this;
            }

            const VerbosityLevel& GetVerbosityLevel( ) const noexcept
            {
                return lvl;
            }

            const wchar_t* GetString( ) const noexcept
            {
                return str.get( );
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
        std::mutex mMsgQueueLock;

        // Worker Thread
        std::thread mWorkerThread;
        std::atomic<bool> mTerminate;
        std::condition_variable mMsgCV;

        /// Private Worker Methods \\\

        // Worker Thread Methods
        void WorkerLogLoop( );
        void WaitForMsgs( );
        bool WaitPredicate( ) const noexcept;
        void PushMsg(LogMessage&& msg);
        LogMessage PopMsg( );
        void LogNextMsg( );

    public:
        /// Constructors \\\

        // Single-ConfigPackage Constructor [C]
        SyncLogger(const ConfigPackage&);

        // Multiple-ConfigPackage Constructor [C]
        SyncLogger(const ConfigPackage&, const ConfigPackage&);

        /// Destructor \\\

        ~SyncLogger( );

        /// Public Methods \\\

        template <class T, ENABLE_IF_SUPPORTED_CHARACTER_TYPE(T)>
        bool Log(const VerbosityLevel& lvl, const T* pFormat, ...);

        template <class T, ENABLE_IF_SUPPORTED_CHARACTER_TYPE(T)>
        bool Log(const VerbosityLevel& lvl, const T* pFormat, va_list pArgs);
    };
}