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

void execute(const GCommand &cmd)
{
}

void executeLinear(const LinearMove &move);
void executeArc(const ArcMove &arc);
void executeSpindle(const SpindleCmd &cmd);
void executeDwell(const DwellCmd &cmd);

// interpolation methods
std::vector<Vec3> interpolateLinear(const Vec3 &from, const Vec3 &to, double stepMm = 0.1);

std::vector<Vec3> interpolateArc(const Vec3 &start, const Vec3 &end, double i, double j, bool clockwise,
                                 double stepDeg = 0.5);