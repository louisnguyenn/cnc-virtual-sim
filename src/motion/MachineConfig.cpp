#include "motion/MachineConfig.h"
#include "simulator/SimulatorException.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

bool MachineConfig::loadFromFile(const std::string &path)
{
    // open file
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw ConfigException("could not open config file: " + path);
    }

    // parse json file
    nlohmann::json config_file;
    try
    {
        file >> config_file;
    }
    catch (const nlohmann::json::parse_error &e)
    {
        throw ConfigException("failed to parse config file: " + std::string(e.what()));
    }

    // load values from json into axis config and machine config structs
    name = config_file["machine_name"];
    x.minMm = config_file["axes"]["x"]["min_mm"];
    x.maxMm = config_file["axes"]["x"]["max_mm"];
    x.maxFeedrateMmpm = config_file["axes"]["x"]["max_feedrate_mmpm"];
    y.minMm = config_file["axes"]["y"]["min_mm"];
    y.maxMm = config_file["axes"]["y"]["max_mm"];
    y.maxFeedrateMmpm = config_file["axes"]["y"]["max_feedrate_mmpm"];
    z.minMm = config_file["axes"]["z"]["min_mm"];
    z.maxMm = config_file["axes"]["z"]["max_mm"];
    z.maxFeedrateMmpm = config_file["axes"]["z"]["max_feedrate_mmpm"];
    spindleMinRpm = config_file["spindle"]["min_rpm"];
    spindleMaxRpm = config_file["spindle"]["max_rpm"];
    accelerationMmS2 = config_file["acceleration_mm_s2"];

    std::cout << "[INFO] Loaded config: " << name << "\n";
    return true;
}
