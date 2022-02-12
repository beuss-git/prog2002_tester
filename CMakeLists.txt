cmake_minimum_required (VERSION 3.8)

project (test_framework)

find_package(OpenGL REQUIRED)

add_subdirectory(external/glad)
add_subdirectory(external/glfw)
add_subdirectory(external/glm)

add_executable (test_framework "src/main.cpp"  "framework/graphics_framework.hpp" "framework/graphics_framework.cpp" "external/include/argparse/argparse.hpp" "src/student.cpp" "src/student.hpp"  )

target_compile_features(test_framework PRIVATE cxx_std_20)
target_link_libraries(test_framework glfw glad glm OpenGL::GL ${CMAKE_DL_LIBS})
target_include_directories(test_framework PRIVATE "external" "external/include")
