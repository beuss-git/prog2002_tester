cmake_minimum_required (VERSION 3.14)

project (Lab1)

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

#[[add_custom_command(
    TARGET Lab1
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Running $<TARGET_FILE:Lab1> ..."
    COMMAND $<TARGET_FILE:Lab1> > out.txt
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out
)

add_test(
    NAME test1 
    COMMAND ${CMAKE_COMMAND} -E compare_files in.txt out.txt
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out
)]]

#execute_process(COMMAND "cmd" "/c $<TARGET_FILE:Lab1> > out.txt" RESULT_VARIABLE CMD_RESULT)
#add_test(NAME MyTestCreate COMMAND $<TARGET_FILE:Lab1> test --arg_test --output output)
#add_test(NAME MyTestCreateCompare COMMAND ${CMAKE_COMMAND} -E compare_files output
		#outputTest)
