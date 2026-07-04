#include "logger/Logger.h"

/// @brief constructor - store current time
Logger::Logger() : m_startTime(std::chrono::steady_clock::now())
{
}

void Logger::log(const Vec3 &pos, double feedrate, const std::string &status)
{
    Record record{std::chrono::steady_clock::now(), pos, feedrate, status};

    // push onto records
    m_records.push_back(record);
}

// write all records to VSC file
void Logger::writeToCSV(const std::string &path) const
{
}

void Logger::printStatus(const Vec3 &pos, double feedrate, const std::string &status) const
{
}

void Logger::printSummary(const MachineState &state) const
{
}

int Logger::recordCount() const
{
}
