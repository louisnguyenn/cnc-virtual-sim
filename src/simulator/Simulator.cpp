#include "simulator/Simulator.h"
#include "simulator/SimulatorException.h"
#include <iostream>
#include <thread>

Simulator::Simulator(const std::string &configPath)
{
    // check if loading is successful
    if (!m_config.loadFromFile(configPath))
    {
        throw SimulatorException("Failed to load config: " + configPath);
    }
}

// getters
const MachineConfig &Simulator::getConfig() const
{
    return m_config;
}

const MachineState &Simulator::getState() const
{
    return m_state;
}

/// @brief parser thread function
/// @param path 
/// @param queue 
void Simulator::parserThread(const std::string &path, CommandQueue<GCommand> &queue)
{

}

/// @brief simulator thread function
/// @param queue 
void Simulator::simulatorThread(CommandQueue<GCommand> &queue)
{

}

/// @brief run a gcode file through the full pipeline
/// @param gcodePath 
void Simulator::run(const std::string &gcodePath)
{

}
