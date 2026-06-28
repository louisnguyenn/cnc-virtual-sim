#include "motion/MotionEngine.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

// constructor
MotionEngine::MotionEngine(MachineState &state) : m_state(state)
{
}

// tool path getter
const std::vector<Vec3> &MotionEngine::getToolPath() const
{
    return m_toolpath;
}

void MotionEngine::execute(const GCommand &cmd)
{
    if (std::holds_alternative<LinearMove>(cmd))
    {
        executeLinear(std::get<LinearMove>(cmd));
    }
    else if (std::holds_alternative<ArcMove>(cmd))
    {
        executeArc(std::get<ArcMove>(cmd));
    }
    else if (std::holds_alternative<SpindleCmd>(cmd))
    {
        executeSpindle(std::get<SpindleCmd>(cmd));
    }
    else if (std::holds_alternative<DwellCmd>(cmd))
    {
        executeDwell(std::get<DwellCmd>(cmd));
    }
    else if (std::holds_alternative<ProgramEnd>(cmd))
    {
        setState(MachineStatus::FINISHED);
        std::cout << "[INFO] Program End\n";
    }
}

void executeLinear(const LinearMove &move);
void executeArc(const ArcMove &arc);
void executeSpindle(const SpindleCmd &cmd);
void executeDwell(const DwellCmd &cmd);

// interpolation methods
std::vector<Vec3> interpolateLinear(const Vec3 &from, const Vec3 &to, double stepMm = 0.1);

std::vector<Vec3> interpolateArc(const Vec3 &start, const Vec3 &end, double i, double j, bool clockwise,
                                 double stepDeg = 0.5);