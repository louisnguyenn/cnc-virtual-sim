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

/// @brief execute a g command based on it's command (LinearMove, ArcMove, etc.)
/// @param cmd
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

/// @brief execute linear move
/// @param move - type LinearMove
void MotionEngine::executeLinear(const LinearMove &move)
{
    Vec3 target{move.x, move.y, move.z};
    Vec3 start = m_state.position;

    auto points = interpolateLinear(start, target);

    // add points to tool path
    for (const auto &pt : points)
    {
        m_toolpath.push_back(pt);
    }

    // update machine state
    double distance = start.distanceTo(target);
    m_state.totalDistance += distance;
    m_state.position = target;
    m_state.feedrate = move.feedrate;

    // calculate cycle time
    if (move.feedrate > 0.0 && !move.rapid)
    {
        m_state.cycleTimeSeconds += (distance / move.feedrate) * 60.0;
    }

    std::cout << "[INFO] LinearMove → X:" << target.x << " Y:" << target.y << " Z:" << target.z
              << (move.rapid ? " (rapid)\n" : "\n");
}

void MotionEngine::executeArc(const ArcMove &arc)
{
}

void MotionEngine::executeSpindle(const SpindleCmd &cmd)
{
}

void MotionEngine::executeDwell(const DwellCmd &cmd)
{
}

// interpolation methods

/// @brief create dotted points from point 1 to point 2. stepMm tell us how fine each points are from eachother
/// @param from
/// @param to
/// @param stepMm
/// @return vector of points
std::vector<Vec3> MotionEngine::interpolateLinear(const Vec3 &from, const Vec3 &to, double stepMm = 0.1)
{
    std::vector<Vec3> points;
    double distance = from.distanceTo(to); // get distance

    // if we're already at the target
    if (distance < 1e-9)
    {
        points.push_back(to);
        return points;
    }

    // calculate steps
    // e.g. dist=50mm, stepMm=0.1 → 500 steps
    int steps = std::max(1, static_cast<int>(distance / stepMm));

    // record each position 't' from start to finish
    // use linear interpolation (lerp) formula
    for (int i{0}; i <= steps; ++i)
    {
        double t = static_cast<double>(i) / steps;
        points.push_back(from + (to - from) *t);
    }

    return points;
}

std::vector<Vec3> MotionEngine::interpolateArc(const Vec3 &start, const Vec3 &end, double i, double j, bool clockwise,
                                               double stepDeg = 0.5)
{
}
