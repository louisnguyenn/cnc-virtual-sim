#include "simulator/Simulator.h"
#include "simulator/SimulatorException.h"
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "CNC Simulator v0.1.0\n";
    std::cout << "====================\n";

    // default paths
    std::string configPath = "config/machine.json";
    std::string gcodePath = "tests/programs/square.gcode";

    // allow passing a gcode file as argument
    if (argc > 1)
        gcodePath = argv[1];

    try
    {
        Simulator sim(configPath);
        sim.run(gcodePath);

        // print summary
        const auto &state = sim.getState();
        std::cout << "\n=== Summary ===\n";
        std::cout << "Final position : X=" << state.position.x << " Y=" << state.position.y << " Z=" << state.position.z
                  << "\n";
        std::cout << "Total distance : " << state.totalDistance << "mm\n";
        std::cout << "Cycle time     : " << state.cycleTimeSeconds << "s\n";
    }
    catch (const SimulatorException &e)
    {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
