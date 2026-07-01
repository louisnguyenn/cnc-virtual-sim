#pragma once
#include "MachineConfig.h"
#include "MachineState.h"
#include "Vec3.h"
#include "parser/GCommand.h"
#include "simulator/SimulatorException.h"
#include <vector>

class MotionEngine
{
  private:
    MachineState &m_state;        // machine state
    std::vector<Vec3> m_toolpath; // tool position
    const MachineConfig &m_config;

    void executeLinear(const LinearMove &move);
    void executeArc(const ArcMove &arc);
    void executeSpindle(const SpindleCmd &cmd);
    void executeDwell(const DwellCmd &cmd);
    void checkLimits(const Vec3 &pos);

    // interpolation methods
    std::vector<Vec3> interpolateLinear(const Vec3 &from, const Vec3 &to, double stepMm = 0.1);

    std::vector<Vec3> interpolateArc(const Vec3 &start, const Vec3 &end, double i, double j, bool clockwise,
                                     double stepDeg = 0.5);

  public:
    explicit MotionEngine(MachineState &state, const MachineConfig &config);

    void setState(MachineStatus state)
    {
        m_state.status = state;
    }

    void execute(const GCommand &cmd); // execute a gcommand

    const std::vector<Vec3> &getToolPath() const; // return tool position
};
