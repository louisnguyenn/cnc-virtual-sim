#include "logger/Logger.h"
#include <fstream>
#include <iostream>

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
    std::ofstream csv_file(path);

    // write header row
    csv_file << "time_ms,x,y,z,feedrate,status\n";

    for (const auto &record : m_records)
    {
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(record.timestamp - m_startTime).count();

        csv_file << ms << ','; // write timestamp

        csv_file << record.pos.x << ',' << record.pos.y << ',' << record.pos.z << ','; // write position
        // std::cout << "X POS: " << record.pos.x << '\n';
        // std::cout << "Y POS: " << record.pos.y << '\n';
        // std::cout << "Z POS: " << record.pos.z << '\n';

        csv_file << record.feedrate << ','; // write feedrate
        // std::cout << "FEEDRATE: " << record.feedrate << '\n';

        csv_file << record.machineStatus << '\n'; // write machine status
    }

    csv_file.close();
}

void Logger::printStatus(const Vec3 &pos, double feedrate, const std::string &status) const
{
    std::cout << "\r[" << status << "] X:" << pos.x << " Y:" << pos.y << " Z:" << pos.z << " F:" << feedrate
              << "          " << std::flush;
}

void Logger::printSummary(const MachineState &state) const
{
    std::cout << "\n========= SUMMARY =========\n";
    std::cout << "Final position  : X=" << state.position.x << " Y=" << state.position.y << " Z=" << state.position.z
              << "\n";
    std::cout << "Total distance  : " << state.totalDistance << "mm\n";
    std::cout << "Cycle time      : " << state.cycleTimeSeconds << "s\n";
    std::cout << "Records logged  : " << recordCount() << "\n";
    std::cout << "===========================\n";
}

int Logger::recordCount() const
{
    return m_records.size();
}
