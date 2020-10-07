cmake_minimum_required(VERSION 3.2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(uv-sanatizer-test)

add_executable(unit_tests unit_tests.cpp ../uv-sanatizer/debounce_task.cpp ../uv-sanatizer/sanatize_task.cpp)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
target_link_libraries(unit_tests Boost::unit_test_framework)