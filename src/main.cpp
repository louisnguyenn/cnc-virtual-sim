#include "motion/MachineConfig.h"
#include "motion/MachineState.h"
#include "motion/MotionEngine.h"
#include "parser/GcodeParser.h"
#include "simulator/SimulatorException.h"
#include <iostream>

int main()
{
    std::cout << "CNC Simulator v0.1.0\n";
    std::cout << "====================\n";

    // load machine config
    MachineConfig config;
    if (!config.loadFromFile("config/machine.json"))
    {
        std::cerr << "Failed to load machine config!\n";
        return 1;
    }

    // parse gcode
    GcodeParser parser;
    auto commands = parser.parseFile("tests/programs/square.gcode");

    if (commands.empty())
    {
        std::cerr << "No commands parsed!\n";
        return 1;
    }

    // run simulation
    MachineState state;
    MotionEngine engine(state, config);

    try
    {
        for (const auto &cmd : commands)
        {
            engine.execute(cmd);
        }
    }
    catch (const MachineAlarmException &e)
    {
        std::cerr << "\n*** " << e.what() << " ***\n";
        state.status = MachineStatus::ALARM;
        return 1;
    }

    // summary
    std::cout << "\n=== Summary ===\n";
    std::cout << "Final position : X=" << state.position.x << " Y=" << state.position.y << " Z=" << state.position.z
              << "\n";
    std::cout << "Total distance : " << state.totalDistance << "mm\n";
    std::cout << "Cycle time     : " << state.cycleTimeSeconds << "s\n";
    std::cout << "Toolpath points: " << engine.getToolPath().size() << "\n";

    return 0;
}
