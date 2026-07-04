#pragma once
#include "motion/MachineState.h"
#include "motion/Vec3.h"
#include <chrono>
#include <string>
#include <vector>

class Logger
{
  public:
    struct Record
    {
        std::chrono::steady_clock::time_point timestamp;
        Vec3 pos;
        double feedrate;
        std::string machineStatus;
    };

  private:
    std::vector<Record> m_records;
    std::chrono::steady_clock::time_point m_startTime;

  public:
    // constructor
    Logger();

    // log every time the machine moves
    void log(const Vec3 &pos, double feedrate, const std::string &status);

    // write all records to VSC file
    void writeToCSV(const std::string &path) const;

    void printStatus(const Vec3 &pos, double feedrate) const;

    void printSummary(const MachineState &state) const;

    int recordCount() const;
};
