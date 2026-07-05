#include "simulator/AppConfig.h"
#include <iostream>

// parse from argc/argv
static AppConfig parse(int argc, char *argv[])
{
    AppConfig app;

    for (auto i{1}; i < argc - 1; ++i)
    {
        if (argv[i] == "--config")
        {

        }

        if (argv[i] == "--input")
        {
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
static void printUsage()
{
}
