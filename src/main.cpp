#include "logger/AppLogger.h"
#include "simulator/AppConfig.h"
#include "simulator/Simulator.h"
#include "simulator/SimulatorException.h"
#include <iostream>

int main(int argc, char *argv[])
{
    // parse command-line args
    auto config = AppConfig::parse(argc, argv);
    if (!config.valid)
        return 1;

    // set up app logger
    auto &log = AppLogger::get();
    log.setLevel(config.verbose ? LogLevel::DEBUG : LogLevel::INFO);
    log.setFile("simulator.log");

    log.info("CNC Simulator v0.1.0 starting");
    log.debug("Config path: " + config.configPath);
    log.debug("Input path:  " + config.inputPath);

    try
    {
        Simulator sim(config.configPath);
        sim.run(config.inputPath);
        sim.getLogger().printSummary(sim.getState());
        std::cout << "Toolpath saved to toolpath.csv\n";
    }
    catch (const ConfigException &e)
    {
        log.error(e.what());
        return 1;
    }
    catch (const ParseException &e)
    {
        log.error(e.what());
        return 1;
    }
    catch (const SimulatorException &e)
    {
        log.error(e.what());
        return 1;
    }

    log.info("Simulation finished successfully");
    return 0;
}
