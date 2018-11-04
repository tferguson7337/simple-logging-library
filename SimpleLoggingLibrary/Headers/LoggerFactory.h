#pragma once

#include "Interfaces\ILogger.h"

namespace SLL
{
    /// Single Configuration [C].
    // Return std::shared_ptr to an object that adheres to the ILogger interface.
    // Specific logger object returned is dependent on provided ConfigPackage argument.
    std::shared_ptr<ILogger> BuildLogger(const ConfigPackage& config);

    /// Multiple Configurations [C].
    // Return std::shared_ptr to an object that adheres to the ILogger interface.
    // Specific logger object returned is dependent on provided ConfigPackage argument.
    std::shared_ptr<ILogger> BuildLogger(const ConfigPackage& stdOutConfig, const ConfigPackage& fileConfig);
}