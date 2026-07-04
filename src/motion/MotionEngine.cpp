#include "motion/MotionEngine.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

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

// constructor
MotionEngine::MotionEngine(MachineState &state, const MachineConfig &config, Logger &logger)
    : m_state(state), m_config(config), m_logger(logger)
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
    Vec3 end{move.x, move.y, move.z};
    Vec3 start{m_state.position};

    checkLimits(end);

    auto points = interpolateLinear(start, end);

    // add points to tool path
    for (const auto &pt : points)
    {
        m_toolpath.push_back(pt);
    }

    // update machine state
    double distance = start.distanceTo(end);
    m_state.totalDistance += distance;
    m_state.position = end;
    m_state.feedrate = move.feedrate;

    // calculate cycle time
    if (move.feedrate > 0.0 && !move.rapid)
    {
        m_state.cycleTimeSeconds += (distance / move.feedrate) * 60.0;
    }

    std::cout << "[INFO] LinearMove → X:" << end.x << " Y:" << end.y << " Z:" << end.z
              << (move.rapid ? " (rapid)\n" : "\n");

    m_logger.log(m_state.position, m_state.feedrate, statusToString(m_state.status));
    m_logger.printStatus(m_state.position, m_state.feedrate, statusToString(m_state.status));
}

/// @brief execute arc move
/// @param arc
void MotionEngine::executeArc(const ArcMove &arc)
{
    Vec3 start{m_state.position};
    Vec3 end{arc.x, arc.y, arc.z};

    checkLimits(end);

    auto points = interpolateArc(start, end, arc.i, arc.j, arc.clockwise);

    // add each point to the toolpath vector
    for (const auto &pt : points)
    {
        m_toolpath.push_back(pt);
    }

    // update machine state
    double distance = start.distanceTo(end);
    m_state.totalDistance += distance;
    m_state.position = end;
    m_state.feedrate = arc.feedrate;

    if (arc.feedrate > 0.0)
    {
        m_state.cycleTimeSeconds += (distance / arc.feedrate) * 60.0;
    }

    std::cout << "[INFO] ArcMove → X:" << end.x << " Y:" << end.y << (arc.clockwise ? " (CW)\n" : " (CCW)\n");
}

/// @brief update spindle machine state
/// @param cmd
void MotionEngine::executeSpindle(const SpindleCmd &cmd)
{
    m_state.spindleOn = cmd.on;
    m_state.spindleRpm = cmd.rpm;

    std::cout << "[INFO] Spindle " << (cmd.on ? "ON" : "OFF") << " at " << cmd.rpm << " RPM\n";
}

/// @brief update dwell machine state
/// @param cmd
void MotionEngine::executeDwell(const DwellCmd &cmd)
{
    m_state.cycleTimeSeconds += cmd.seconds;

    std::cout << "[INFO] Dwell " << cmd.seconds << "s\n";
}

// interpolation methods

/// @brief create dotted points from point 1 to point 2. stepMm tell us how fine each points are from eachother
/// @param from
/// @param to
/// @param stepMm
/// @return vector of points
std::vector<Vec3> MotionEngine::interpolateLinear(const Vec3 &from, const Vec3 &to, double stepMm)
{
    std::vector<Vec3> points;
    double distance = from.distanceTo(to); // get distance

    // if we're already at the end
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
        points.push_back(from + (to - from) * t);
    }

    return points;
}

std::vector<Vec3> MotionEngine::interpolateArc(const Vec3 &start, const Vec3 &end, double i, double j, bool clockwise,
                                               double stepDeg)
{
    std::vector<Vec3> points;

    // centre of the arc = start position * (i, j)
    // i and j are the distance from the start point to the centre
    Vec3 centre{start.x + i, start.y + j, start.z};

    // radius
    double r{std::hypot(i, j)};

    // angle from centre to start point
    double a0{std::atan2(start.y - centre.y, start.x - centre.x)};

    // angle from centre to end point
    double a1{std::atan2(end.y - centre.y, end.x - centre.x)};

    // rotation - cw means angle decrease, ccw means angle increase
    if (clockwise && a1 > a0)
    {
        a1 -= 2.0 * M_PI;
    }
    if (!clockwise && a1 < a0)
    {
        a1 += 2.0 * M_PI;
    }

    double stepRag = stepDeg * M_PI / 180.0; // convert to radians

    // calculate how many steps to cover the full arc angle
    int steps{std::max(1, static_cast<int>(std::abs(a1 - a0) / stepRag))};

    for (int s{0}; s <= steps; ++s)
    {
        double t = static_cast<double>(s) / steps;
        double a = a0 + (a1 - a0) * t;

        /*
        convert polar to cartesian form using formula:
        X = radius * cos(angle)
        Y = radius * sin(angle)*/
        // push x and y points on the circle. z stays the same
        points.push_back({centre.x + r * std::cos(a), centre.y + r * std::sin(a), start.z});
    }

    return points;
}

/// @brief check axis limits and throw exception
/// @param pos
void MotionEngine::checkLimits(const Vec3 &pos)
{
    if (pos.x < m_config.x.minMm || pos.x > m_config.x.maxMm)
        throw MachineAlarmException("X overtravel at " + std::to_string(pos.x) + " (limits: " +
                                    std::to_string(m_config.x.minMm) + " to " + std::to_string(m_config.x.maxMm) + ")");

    if (pos.y < m_config.y.minMm || pos.y > m_config.y.maxMm)
        throw MachineAlarmException("Y overtravel at " + std::to_string(pos.y) + " (limits: " +
                                    std::to_string(m_config.y.minMm) + " to " + std::to_string(m_config.y.maxMm) + ")");

    if (pos.z < m_config.z.minMm || pos.z > m_config.z.maxMm)
        throw MachineAlarmException("Z overtravel at " + std::to_string(pos.z) + " (limits: " +
                                    std::to_string(m_config.z.minMm) + " to " + std::to_string(m_config.z.maxMm) + ")");
}
