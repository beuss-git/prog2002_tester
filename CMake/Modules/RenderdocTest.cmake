include(CMake/Modules/TestManager.cmake)

find_library(RENDERDOC_LIBRARY NAMES renderdoc REQUIRED)

include(FetchContent)
# fmt
FetchContent_Declare(
	fmt
	GIT_REPOSITORY https://github.com/fmtlib/fmt.git
	GIT_TAG 8.1.1)
FetchContent_MakeAvailable(fmt)

# magicenum
FetchContent_Declare(
	magic_enum
	GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
	GIT_TAG v0.7.3)
FetchContent_MakeAvailable(magic_enum)


if (MSVC)
	add_compile_definitions(RENDERDOC_PLATFORM_WIN32)
else()
	# Needed to export (and find) renderdoc__replay__marker on linux
	set(CMAKE_ENABLE_EXPORTS ON)
	add_compile_definitions(RENDERDOC_PLATFORM_LINUX)
endif()

# KIT = TestDriverName
function(create_test_driver KIT)
	set(ADDITIONAL_SRC ${ARGN} 
		"src/testing/renderdoc_helper.cpp" 
		"src/testing/renderdoc_helper.hpp"
		"src/testing/tester.cpp"
		"src/testing/tester.hpp"
	)

	create_test_sourcelist(Tests ${KIT}TestDriver.cxx "src/testing/renderdoc_test.cpp")

	add_executable(${KIT}TestDriver ${KIT}TestDriver.cxx ${Tests} ${ADDITIONAL_SRC})

	target_include_directories(${KIT}TestDriver PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/external
	)

	target_link_libraries(${KIT}TestDriver PRIVATE ${RENDERDOC_LIBRARY} fmt::fmt argparse::argparse magic_enum::magic_enum)

	target_compile_features(${KIT}TestDriver PRIVATE cxx_std_17)
endfunction()



# TODO: make work properly without specifying FRAME and FRAME_COUNT
function(add_renderdoc_test TEST_NAME KIT)

	check_test_enabled()
	if (NOT ${TEST_ENABLED})
		#message(WARNING "Test ${TEST_NAME} is not enabled in EnabledTests.txt")
		return()
	endif()

	# Let the student know that it's enabled
	message(STATUS "Test ${TEST_NAME} enabled in EnabledTests.txt")

	cmake_parse_arguments(
		PARSED
		""
		"FRAME;FRAME_COUNT"
		""
		${ARGN}
	)

	if (NOT PARSED_FRAME)
		set(PARSED_FRAME 0) # Default to first frame
	endif(NOT PARSED_FRAME)

	if (NOT PARSED_FRAME_COUNT)
		set(PARSED_FRAME_COUNT 1) # Default to 1 frame
	endif(NOT PARSED_FRAME_COUNT)

	# Path to the executable we want to capture
	set(EXE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT})

	# renderdoccmd executable
	set(CMD_BIN renderdoccmd)

	# arguments to pass on to the executable we want to capture
	set(CMD_ARGS "capture -w -c ${KIT} ${EXE_PATH} --test --frame ${PARSED_FRAME} --framecount ${PARSED_FRAME_COUNT}")

	set(TESTDRIVER_BIN "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}TestDriver")

	# Probably a better way to do this.. but this works
	string (REPLACE ";" " " PARSED_UNPARSED_ARGUMENTS "${PARSED_UNPARSED_ARGUMENTS}")

	set(TESTDRIVER_TEST_NAME "src/testing/renderdoc_test")

	# Pass on PARSED_UNPARSED_ARGUMENTS to ignore the parsed arguments
	# we don't want to pass them to the executable
	set(TESTDRIVER_ARGS "${TESTDRIVER_TEST_NAME} ${PARSED_UNPARSED_ARGUMENTS} --capture_file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}_capture.rdc")


	add_test(NAME ${TEST_NAME} COMMAND 
			${CMAKE_COMMAND}
				-DCMD1=${CMD_BIN}				# Launches test binary through renderdoccmd
				-DCMD1ARGS=${CMD_ARGS}
				-DCMD2=${TESTDRIVER_BIN}		# Executes the test driver
				-DCMD2ARGS=${TESTDRIVER_ARGS}
			-P ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules/RunTests.cmake
	)

endfunction()
