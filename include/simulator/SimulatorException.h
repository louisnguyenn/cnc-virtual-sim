#pragma once
#include <stdexcept>
#include <string>

// general base exception
class SimulatorException : public std::runtime_error
{
  public:
    explicit SimulatorException(const std::string &msg) : std::runtime_error(msg)
    {
    }
};

// throw when a machine move exceeds axis limit
class MachineAlarmException : public SimulatorException
{
  public:
    explicit MachineAlarmException(const std::string &msg) : SimulatorException("ALARM: " + msg)
    {
    }
};
