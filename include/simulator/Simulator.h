#pragma once
#include "CommandQueue.h"
#include "logger/Logger.h"
#include "motion/MachineConfig.h"
#include "motion/MachineState.h"
#include "motion/MotionEngine.h"
#include "parser/GCommand.h"
#include "parser/GcodeParser.h"
#include <atomic>
#include <string>
#include <thread>

class Simulator
{
  private:
    MachineConfig m_config;
    MachineState m_state;
    Logger m_logger;

    // parser thread function
    void parserThread(const std::string &path, CommandQueue<GCommand> &queue);

    // simulator thread function
    void simulatorThread(CommandQueue<GCommand> &queue);

  public:
    explicit Simulator(const std::string &configPath, Logger &logger);

    // run a gcode file through the full pipeline
    void run(const std::string &gcodePath);

    // getters for config and state
    const MachineConfig &getConfig() const;
    const MachineState &getState() const;
    const Logger &getLogger() const;
};
