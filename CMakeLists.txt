cmake_minimum_required (VERSION 3.8)

project (test_framework)

find_package(OpenGL REQUIRED)

add_subdirectory(external/glad)
add_subdirectory(external/glfw)
add_subdirectory(external/glm)
#add_subdirectory(${PROJECT_SOURCE_DIR}/external/pugixml)
add_subdirectory(external/pugixml)
add_subdirectory(external/fmt)
add_subdirectory(external/miniz)


add_executable (test_framework 
	"src/main.cpp"  
	"framework/graphics_framework.hpp" 
	"framework/graphics_framework.cpp" 
	"external/include/argparse/argparse.hpp" 
	"src/student.cpp"
	"src/student.hpp"
	"framework/renderdoc.hpp"
	"framework/renderdoc.cpp"
	
 )

#add_compile_definitions(RENDERDOC_EXPORTS)
if (MSVC)
	add_compile_definitions(RENDERDOC_PLATFORM_WIN32)
else()
	add_compile_definitions(RENDERDOC_PLATFORM_LINUX)
endif()

target_compile_features(test_framework PRIVATE cxx_std_17)
target_link_libraries(test_framework glfw glad glm OpenGL::GL pugixml fmt miniz)
target_include_directories(test_framework PRIVATE "external" "external/include" "external/pugixml/src")

add_subdirectory(tests)

