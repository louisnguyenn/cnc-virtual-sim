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

    // getters
    const MachineConfig &Simulator::getConfig() const
    {
        return m_config;
    }

    const MachineState &Simulator::getState() const
    {
        return m_state;
    }
}