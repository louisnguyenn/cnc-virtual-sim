#include "simulator/AppConfig.h"
#include <iostream>

// parse from argc/argv
AppConfig AppConfig::parse(int argc, char *argv[])
{
    AppConfig app;

    for (auto i{1}; i < argc - 1; ++i)
    {
        if (argv[i] == "--config")
        {
            app.configPath = argv[i];
        }

        if (argv[i] == "--input")
        {
            app.inputPath = argv[i];
        }

        if (argv[i] == "--verbose")
        {
            app.verbose = true;
        }

        if (argv[i] == "--help")
        {
            printUsage();

            app.valid = false;
        }
        else
        {
            std::cerr << "Error: unknown argument " << argv[i] << '\n';

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
                 "tests/programs/square.gcode)\n\t--verbose\tEnable debug logging\n--help\tShow this message";
}
