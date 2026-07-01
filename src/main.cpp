#include "motion/MachineState.h"
#include "motion/MotionEngine.h"
#include "parser/GcodeParser.h"
#include <iostream>

int main()
{
    std::cout << "CNC Simulator v0.1.0\n";
    std::cout << "====================\n";

    // parse gcode file
    GcodeParser parser;
    auto commands = parser.parseFile("tests/programs/square.gcode");

    if (commands.empty())
    {
        std::cerr << "No commands parsed!\n";
        return 1;
    }

    // Create machine state and motion engine
    MachineState state;
    MotionEngine engine(state);

    // Execute every command
    for (const auto &cmd : commands)
    {
        engine.execute(cmd);
    }

    // Print summary
    std::cout << "\n=== Summary ===\n";
    std::cout << "Final position : X=" << state.position.x << " Y=" << state.position.y << " Z=" << state.position.z
              << "\n";
    std::cout << "Total distance : " << state.totalDistance << "mm\n";
    std::cout << "Cycle time     : " << state.cycleTimeSeconds << "s\n";
    std::cout << "Toolpath points: " << engine.getToolPath().size() << "\n";

    return 0;
}
