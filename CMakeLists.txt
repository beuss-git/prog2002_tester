cmake_minimum_required (VERSION 3.8)

project(tester)

# Enable testing
enable_testing()

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


# Dev executable
add_executable(tester_dev 
	"src/main.cpp"
	"src/testing/renderdoc_helper.cpp"
	"src/testing/renderdoc_helper.hpp"
	"src/testing/tester.cpp"
	"src/testing/tester.hpp"
)

target_include_directories(tester_dev PRIVATE "external" "external/argparse/include")

target_compile_features(tester_dev PRIVATE cxx_std_17)

target_link_libraries(tester_dev PRIVATE ${RENDERDOC_LIBRARY} fmt::fmt argparse::argparse magic_enum::magic_enum)

# Export
set(TESTER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)
