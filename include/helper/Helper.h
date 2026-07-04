#include "motion/MotionEngine.h"
#include <string>

/// @brief machine status to string type conversion
/// @param status
/// @return string
static std::string statusToString(MachineStatus status)
{
    switch (status)
    {
    case MachineStatus::IDLE:
        return "IDLE";
    case MachineStatus::HOLD:
        return "HOLD";
    case MachineStatus::ALARM:
        return "ALARM";
    case MachineStatus::RUNNING:
        return "RUNNING";
    case MachineStatus::FINISHED:
        return "FINISHED";
    default:
        return "UNKNOWN";
    }
}
