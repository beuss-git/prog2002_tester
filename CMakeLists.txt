cmake_minimum_required (VERSION 3.8)

project(TESTER)

# Enable testing
enable_testing()

# Define the tests
include ("modules/Tests.cmake")

# Add argparse for framework library
add_subdirectory(external/argparse)

# Create the framework library
add_library(${PROJECT_NAME} "src/framework.cpp" "include/framework.hpp")

# Enable C++17 for compilers that has older defaults
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Set include dirs
target_include_directories(${PROJECT_NAME} PRIVATE "external" "external/argparse/include")
