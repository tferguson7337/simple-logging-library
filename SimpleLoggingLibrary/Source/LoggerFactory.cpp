#include <LoggerFactory.h>

#include <StreamLogger.h>
#include <DualLogger.h>
#include <AsyncLogger.h>

namespace SLL
{
    /// Single Configuration.
    // Return std::shared_ptr to an object that adheres to the ILogger interface.
    // Specific logger object returned is dependent on provided ConfigPackage argument.
    std::shared_ptr<ILogger> BuildLogger(const ConfigPackage& config)
    {
        return BuildLogger(config, config);
    }

    /// Multiple Configurations.
    // Return std::shared_ptr to an object that adheres to the ILogger interface.
    // Specific logger object returned is dependent on provided ConfigPackage argument.
    std::shared_ptr<ILogger> BuildLogger(const ConfigPackage& stdOutConfig, const ConfigPackage& fileConfig)
    {
        const OptionFlag& stdOutMask = stdOutConfig.GetOptionFlags( );
        const OptionFlag& fileMask = fileConfig.GetOptionFlags( );
        const OptionFlag  compositeMask = stdOutMask | fileMask;

        if ( (compositeMask & (OptionFlag::LogToStdout | OptionFlag::LogToFile)) == OptionFlag::NONE )
        {
            throw std::logic_error(__FUNCTION__" - Attempted to build logger without LogToStdout or LogToFile options enabled.");
        }

        if ( (compositeMask & OptionFlag::LogAsynchronous) == OptionFlag::LogAsynchronous )
        {
            if ( (compositeMask & (OptionFlag::LogToStdout | OptionFlag::LogToFile)) == (OptionFlag::LogToStdout | OptionFlag::LogToFile) )
            {
                return std::make_shared<AsyncLogger>(stdOutConfig, fileConfig);
            }
            else if ( (compositeMask & (OptionFlag::LogToStdout)) == OptionFlag::LogToStdout )
            {
                return std::make_shared<AsyncLogger>(stdOutConfig);
            }
            else if ( (compositeMask & (OptionFlag::LogToFile)) == OptionFlag::LogToFile )
            {
                return std::make_shared<AsyncLogger>(fileConfig);
            }
        }
        else
        {
            if ( (compositeMask & (OptionFlag::LogToStdout | OptionFlag::LogToFile)) == (OptionFlag::LogToStdout | OptionFlag::LogToFile) )
            {
                return std::make_shared<DualLogger>(stdOutConfig, fileConfig);
            }
            else if ( (compositeMask & (OptionFlag::LogToStdout)) == OptionFlag::LogToStdout )
            {
                return std::make_shared<StdOutLogger>(stdOutConfig);
            }
            else if ( (compositeMask & (OptionFlag::LogToFile)) == OptionFlag::LogToFile )
            {
                return std::make_shared<FileLogger>(fileConfig);
            }
        }

        throw std::runtime_error(__FUNCTION__" - Failed to build logger due to unknown configurations.");
    }
}