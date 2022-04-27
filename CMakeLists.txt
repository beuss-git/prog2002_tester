cmake_minimum_required (VERSION 3.14)

set(PROJECT_NAME Lab1)
project (${PROJECT_NAME})

# Choose vendor neutral OpenGL (libOpenGL.so, libGLX.so) over libGL.so
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_subdirectory(external/glad)
add_subdirectory(external/glfw)
add_subdirectory(external/glm)
add_subdirectory(external/argparse)

add_executable (${PROJECT_NAME} 
	"src/main.cpp"  
	"framework/graphics_framework.hpp" 
	"framework/graphics_framework.cpp" 
	"src/student.cpp"
	"src/student.hpp"
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad glm OpenGL::GL)
target_include_directories(${PROJECT_NAME} PRIVATE "external" "external/argparse/include")

enable_testing()

add_subdirectory(tests)
