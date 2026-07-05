#include "simulator/AppConfig.h"
#include <iostream>
#include <cstring>

// parse from argc/argv
AppConfig AppConfig::parse(int argc, char *argv[])
{
    AppConfig app;

    for (auto i{1}; i < argc - 1; ++i)
    {
        if (strcmp(argv[i],"--config"))
        {
            app.configPath = argv[i];
        }

        if (strcmp(argv[i],"--input"))
        {
            app.inputPath = argv[i];
        }

        if (strcmp(argv[i],"--verbose"))
        {
            app.verbose = true;
        }

        if (strcmp(argv[i],"--help"))
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
