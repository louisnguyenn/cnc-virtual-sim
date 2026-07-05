#pragma once
#include <string>

struct AppConfig
{
    std::string configPath{"config/machine.json"};
    std::string inputPath{"tests/programs/square.gcode"};
    bool verbose{false};
    bool valid{true};   // false if parsing failed

    // parse from argc/argv
    static AppConfig parse(int argc, char* argv[]);

    // print usage instructions
    static void printUsage();
};
