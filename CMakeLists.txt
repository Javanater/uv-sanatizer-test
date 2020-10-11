cmake_minimum_required(VERSION 3.2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(uv-sanatizer-test)
enable_testing()

# Allow short runs during automated testing to see if something new breaks
set(FUZZ_RUNTIME
    10
    CACHE STRING "Number of seconds to run fuzz tests during ctest run") # Default of 10 seconds

add_executable(unit_tests
    unit_tests.cpp
    ../uv-sanatizer/debounce_task.cpp
    ../uv-sanatizer/debounce_task.hpp
    ../uv-sanatizer/ints.hpp
    ../uv-sanatizer/rising_edge_task.cpp
    ../uv-sanatizer/rising_edge_task.hpp
    ../uv-sanatizer/sanatize_task.cpp
    ../uv-sanatizer/sanatize_task.hpp
)

target_compile_definitions(unit_tests PUBLIC TESTING)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
target_link_libraries(unit_tests Boost::unit_test_framework)
add_test(NAME unit_test_run COMMAND unit_tests)

add_executable(fuzz_tester fuzz_tester.cpp
../uv-sanatizer/debounce_task.cpp
../uv-sanatizer/debounce_task.hpp)
target_link_libraries(
  fuzz_tester
  PRIVATE -coverage
          -fsanitize=fuzzer,undefined,address)
target_compile_options(fuzz_tester PRIVATE -fsanitize=fuzzer,undefined,address)
set_property(TARGET fuzz_tester PROPERTY CXX_STANDARD 17)

add_test(NAME fuzz_tester_run COMMAND fuzz_tester -max_total_time=${FUZZ_RUNTIME})

add_executable(fuzz_sanatize fuzz_sanatize.cpp
../uv-sanatizer/ints.hpp
../uv-sanatizer/sanatize_task.cpp
../uv-sanatizer/sanatize_task.hpp)
target_link_libraries(
    fuzz_sanatize
  PRIVATE -coverage
          -fsanitize=fuzzer,undefined,address)
target_compile_options(fuzz_sanatize PRIVATE -fsanitize=fuzzer,undefined,address)
set_property(TARGET fuzz_sanatize PROPERTY CXX_STANDARD 17)
add_test(NAME fuzz_sanatize_run COMMAND fuzz_sanatize -max_total_time=${FUZZ_RUNTIME})