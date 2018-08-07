#include <FileLogger.h>

namespace SLL
{
    /// Private Helper Methods \\\

    void FileLogger::InitializeFileStream( )
    {
        static const std::string commonThrowStr(__FUNCTION__" - Failed to initialize file stream: ");

        const ConfigPackage& config = GetConfig( );

        if ( config.GetFile( ).empty( ) )
        {
            throw std::invalid_argument(commonThrowStr + "No filename provided.");
        }

        try
        {
            mFileStream.open(config.GetFile( ), std::ios_base::app | std::ios_base::binary | std::ios_base::out);
        }
        catch ( const std::exception& e )
        {
            throw std::runtime_error(commonThrowStr + e.what( ));
        }

        if ( !mFileStream.good( ) || !mFileStream.is_open( ) )
        {
            throw std::runtime_error(
                commonThrowStr +
                "good == " + 
                std::to_string(mFileStream.good( )) + 
                ", is_open == " +
                std::to_string(mFileStream.is_open( ))
            );
        }
    }

    /// Constructors \\\
            
    // ConfigPackage Constructor [C]
    FileLogger::FileLogger(const ConfigPackage& config) :
        LoggerBase(config)
    {
        InitializeFileStream( );
    }

    // ConfigPackage Constructor [M]
    FileLogger::FileLogger(ConfigPackage&& config) :
        LoggerBase(std::move(config))
    {
        InitializeFileStream( );
    }

    // Move Constructor
    FileLogger::FileLogger(FileLogger&& src) noexcept :
        LoggerBase(std::move(src)),
        mFileStream(std::move(src.mFileStream))
    { }

    /// Destructor \\\

    // Virtual Destructor
    FileLogger::~FileLogger( )
    {
        if ( mFileStream.good( ) && mFileStream.is_open( ) )
        {
            try
            {
                mFileStream.flush( );
            }
            catch ( const std::exception& )
            {
                // Best attempt - nothing more to be done.
            }
        }
    }

    /// Assignment Overload \\\

    // Move Assignment
    FileLogger& FileLogger::operator=(FileLogger&& src)
    {
        if ( this == &src )
        {
            throw std::invalid_argument(__FUNCTION__" - Attempted self-assignment.");
        }
        
        LoggerBase::operator=(std::move(src));
        mFileStream = std::move(src.mFileStream);

        return *this;
    }

    /// Public Method \\\

    // Flush buffer contents to file.
    void FileLogger::Flush( )
    {
        if ( mFileStream.good( ) && mFileStream.is_open( ) )
        {
            mFileStream.flush( );
        }
    }

    // Log formatted message to file.
    template <class T, typename>
    bool FileLogger::Log(const VerbosityLevel lvl, const T* pFormat, ...)
    {
        /// Implementation Pending \\\
        
        return false;
    }
}