#pragma once
#include "Vec3.h"
#include <string>

// machine states
enum class MachineStatus
{
    IDLE,
    RUNNING,
    HOLD,
    ALARM,
    FINISHED
};

struct MachineState
{
    Vec3 position;         // current x, y, z position
    double feedrate{0.0};  // current feedrate
    bool spindleOn{false}; // spindle on/off
    int spindleRpm{0};     // spindle speed
    MachineStatus status{MachineStatus::IDLE};
    double totalDistance{0.0};    // total distance travelled
    double cycleTimeSeconds{0.0}; // total simulated time
};
