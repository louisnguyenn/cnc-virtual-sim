add_executable(cnc_tests
    test.cpp
)

target_link_libraries(cnc_tests PRIVATE GTest::gtest_main)
target_include_directories(cnc_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(cnc_tests)