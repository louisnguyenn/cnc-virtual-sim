#include "logger/Logger.h"
#include "motion/MachineConfig.h"
#include "motion/MachineState.h"
#include "motion/MotionEngine.h"
#include "simulator/SimulatorException.h"
#include <gtest/gtest.h>

// helper — creates a default config
// MachineConfig makeTestConfig()
// {
//     MachineConfig config;
//     config.loadFromFile("/home/louis/projects/cnc-virtual-sim/config/machine.json"); // absolute
//     value return config;
// }

// no file needed for testing
MachineConfig makeTestConfig()
{
    MachineConfig config;
    config.name = "TestMachine";
    config.x.minMm = -500.0;
    config.x.maxMm = 500.0;
    config.x.maxFeedrateMmpm = 10000.0;
    config.y.minMm = -400.0;
    config.y.maxMm = 400.0;
    config.y.maxFeedrateMmpm = 10000.0;
    config.z.minMm = -300.0;
    config.z.maxMm = 0.0;
    config.z.maxFeedrateMmpm = 5000.0;
    config.spindleMinRpm = 100;
    config.spindleMaxRpm = 12000;
    config.accelerationMmS2 = 500.0;
    return config;
}

TEST(MotionTest, LinearInterpolationPointCount)
{
    MachineState state;
    MachineConfig config = makeTestConfig(); // call helper
    Logger logger;
    MotionEngine engine(state, config, logger);

    LinearMove move;
    move.x = 10.0; // 10mm move (step default is 0.1mm -> expect 101 steps)
    move.y = 0.0;
    move.z = 0.0;
    move.feedrate = 300.0;
    move.rapid = false;

    engine.execute(move);

    EXPECT_GT(engine.getToolPath().size(),
              1); // should produce 101 steps (should be greater than 1)
}

TEST(MotionTest, MachinePositionUpdated)
{
    MachineState state;
    MachineConfig config = makeTestConfig();
    Logger logger;
    MotionEngine engine(state, config, logger);

    LinearMove move;
    move.x = 50.0;
    move.y = 30.0;
    move.z = 0.0;
    move.feedrate = 300.0;
    move.rapid = false;

    engine.execute(move);

    // check if machine is at the position
    EXPECT_DOUBLE_EQ(state.position.x, 50.0);
    EXPECT_DOUBLE_EQ(state.position.y, 30.0);
}

TEST(MotionTest, OvertravelThrowsAlarm)
{
    MachineState state;
    MachineConfig config = makeTestConfig();
    Logger logger;
    MotionEngine engine(state, config, logger);

    LinearMove move;
    move.x = 9999.0;
    move.y = 0.0;
    move.z = 0.0;
    move.feedrate = 300.0;
    move.rapid = false;

    EXPECT_THROW({ engine.execute(move); }, MachineAlarmException);
}

TEST(MotionTest, CycleTimeIncreases)
{
    MachineState state;
    MachineConfig config = makeTestConfig();
    Logger logger;
    MotionEngine engine(state, config, logger);

    LinearMove move;
    move.x = 50.0;
    move.y = 0.0;
    move.z = 0.0;
    move.feedrate = 300.0;
    move.rapid = false;

    engine.execute(move);

    EXPECT_GT(state.cycleTimeSeconds, 0.0);
}
