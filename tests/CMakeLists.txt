add_executable(cnc_tests
    test_parser.cpp
)

target_link_libraries(cnc_tests PRIVATE
    GTest::gtest_main
    nlohmann_json::nlohmann_json
)

target_include_directories(cnc_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_sources(cnc_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/src/parser/GcodeParser.cpp)

include(GoogleTest)
gtest_add_tests(TARGET cnc_tests)
