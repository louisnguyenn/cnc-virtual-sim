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
    GcodeParser parser;
    auto commands = parser.parseFile(path);

    // push every command onto the queue one by one
    for (auto &cmd : commands)
    {
        queue.push(cmd);
    }

    queue.done();
    std::cout << "[Parser] Finished pushing " << commands.size() << " commands\n";
}

/// @brief simulator thread function
/// @param queue
void Simulator::simulatorThread(CommandQueue<GCommand> &queue)
{
    MotionEngine engine(m_state, m_config, m_logger);

    try
    {
        while (true)
        {
            // pop commands out of the queue
            auto cmd = queue.pop();

            // nullopt means queue is empty
            if (!cmd.has_value())
            {
                break;
            }

            engine.execute(*cmd);
        }
    }
    catch (const MachineAlarmException &e)
    {
        std::cerr << "\n*** " << e.what() << " ***\n";
        m_state.status = MachineStatus::ALARM;
    }

    std::cout << "[Simulator] Finished executing commands\n";
}

/// @brief runs both threads into action
/// @param gcodePath
void Simulator::run(const std::string &gcodePath)
{
    CommandQueue<GCommand> queue;

    m_state.status = MachineStatus::RUNNING;

    std::cout << "[INFO] Starting simulation: " << gcodePath << "\n";

    // launch both threads
    std::thread parser(&Simulator::parserThread, this, std::ref(gcodePath), std::ref(queue));

    std::thread sim(&Simulator::simulatorThread, this, std::ref(queue));

    // wait for both threads to finish before returning
    parser.join();
    sim.join();

    if (m_state.status != MachineStatus::ALARM)
    {
        m_state.status = MachineStatus::FINISHED;
    }

    m_logger.writeToCSV("toolpath.csv");

    std::cout << "[INFO] Simulation complete\n";
}
