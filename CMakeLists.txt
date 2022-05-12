cmake_minimum_required (VERSION 3.8)

project(tester)

# Define the tests
include ("CMake/Modules/Tests.cmake")

# Add argparse for framework library
add_subdirectory(external/argparse)

# Create the framework library
add_library(${PROJECT_NAME} "src/graphics_framework.cpp")

# Enable C++17 for compilers that has older defaults
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Set include dirs
target_include_directories(${PROJECT_NAME} PRIVATE "external" "external/argparse/include")

set(TESTER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)