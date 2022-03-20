cmake_minimum_required (VERSION 3.8)

set(PROJECT_NAME Lab1)
project (${PROJECT_NAME})

find_package(OpenGL REQUIRED)

add_subdirectory(external/glad)
add_subdirectory(external/glfw)
add_subdirectory(external/glm)
add_subdirectory(external/fmt)

add_executable (${PROJECT_NAME} 
	"src/main.cpp"  
	"framework/graphics_framework.hpp" 
	"framework/graphics_framework.cpp" 
	"external/include/argparse/argparse.hpp" 
	"src/student.cpp"
	"src/student.hpp"
)


target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} glfw glad glm OpenGL::GL)
target_include_directories(${PROJECT_NAME} PRIVATE "external" "external/include")

add_subdirectory(tests)

