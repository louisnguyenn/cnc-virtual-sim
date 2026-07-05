#include "simulator/AppConfig.h"
#include <cstring>
#include <iostream>

// parse from argc/argv
AppConfig AppConfig::parse(int argc, char *argv[])
{
    AppConfig app;

    for (auto i{1}; i < argc; ++i)
    {
        if (std::string(argv[i]) == "--config")
        {
            app.configPath = argv[++i];
        }
        else if (std::string(argv[i]) == "--input")
        {
            app.inputPath = argv[++i];
        }
        else if (std::string(argv[i]) == "--verbose")
        {
            app.verbose = true;
        }
        else if (std::string(argv[i]) == "--help")
        {
            printUsage();
            app.valid = false;
        }
        else
        {
            std::cerr << "Unknown argument: " << argv[i] << "\n";
            app.valid = false;
        }
    }

    return app;
}

// print usage instructions
void AppConfig::printUsage()
{
    std::cout << "Usage: cnc_simulator [options]\nOptions:\n\t--config <path>\tPath to machine config JSON (default: "
                 "config/machine.json)\n\t--input <path>\tPath to G-code file (default: "
                 "tests/programs/square.gcode)\n\t--verbose\tEnable debug logging\n\t--help\t\tShow this message\n";
}
