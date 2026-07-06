add_executable(cnc_tests
    test_parser.cpp
    test_motion.cpp
)

target_link_libraries(cnc_tests PRIVATE
    GTest::gtest_main
    nlohmann_json::nlohmann_json
)

target_include_directories(cnc_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_sources(cnc_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/src/parser/GcodeParser.cpp
    ${PROJECT_SOURCE_DIR}/src/motion/MotionEngine.cpp
    ${PROJECT_SOURCE_DIR}/src/motion/MachineConfig.cpp
    ${PROJECT_SOURCE_DIR}/src/logger/Logger.cpp
    ${PROJECT_SOURCE_DIR}/src/logger/AppLogger.cpp)

include(GoogleTest)
gtest_add_tests(TARGET cnc_tests)
