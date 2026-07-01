#pragma once
#include <string>

struct AxisConfig
{
    double minMm;
    double maxMm;
    double maxFeedrateMmpm;
};

struct MachineConfig
{
    std::string name;
    AxisConfig x;
    AxisConfig y;
    AxisConfig z;
    int spindleMinRpm;
    int spindleMaxRpm;
    double accelerationMmS2;

    /// @brief load from json file
    /// @param path
    /// @return true if exists
    bool loadFromFile(const std::string &path);
};
