
find_library(RENDERDOC_LIBRARY NAMES renderdoc REQUIRED)

include(FetchContent)
FetchContent_Declare(
	fmt
	GIT_REPOSITORY https://github.com/fmtlib/fmt.git
	GIT_TAG 8.1.1)
FetchContent_MakeAvailable(fmt)



if (MSVC)
	add_compile_definitions(RENDERDOC_PLATFORM_WIN32)
else()
	# Needed to export (and find) renderdoc__replay__marker on linux
	set(CMAKE_ENABLE_EXPORTS ON)
	add_compile_definitions(RENDERDOC_PLATFORM_LINUX)
endif()

# KIT = TestDriverName
# KIT_LIBS = Libraries
# KitTests = sourcefile for test(s)
function(create_test_driver KIT KIT_LIBS KitTests)
	#set(CMAKE_TESTDRIVER_BEFORE_TESTMAIN "${emscripten_before}#include \"itkTestDriverBeforeTest.inc\"")
	#set(CMAKE_TESTDRIVER_AFTER_TESTMAIN "#include \"itkTestDriverAfterTest.inc\"${emscripten_after}")

	set(ADDITIONAL_SRC ${ARGN} "renderdoc_helper.cpp" "renderdoc_helper.hpp")

	create_test_sourcelist(Tests ${KIT}TestDriver.cxx ${KitTests})

	add_executable(${KIT}TestDriver ${KIT}TestDriver.cxx ${Tests} ${ADDITIONAL_SRC})

	target_include_directories(${KIT}TestDriver PRIVATE
		${CMAKE_SOURCE_DIR}/tests 
	)

	target_link_libraries(${KIT}TestDriver PRIVATE ${KIT_LIBS} ${RENDERDOC_LIBRARY} fmt::fmt argparse::argparse)

	target_compile_features(${KIT}TestDriver PRIVATE cxx_std_17)
endfunction()


# Sets TEST_ENABLED if match is found in EnabledTests.txt
function(check_test_enabled)
	# Read in the file
	file(READ ${CMAKE_SOURCE_DIR}/Labs/EnabledTests.txt TESTS_INPUT)

	set(TEST_ENABLED OFF PARENT_SCOPE)

	# I have to match line by line because ^ is broken when reading the file like this
	STRING(REGEX REPLACE "\r?\n" ";" TESTS_INPUT "${TESTS_INPUT}")
	foreach(line ${TESTS_INPUT})
		if("${line}" MATCHES "^[\\s]*${TEST_NAME}$")
			set(TEST_ENABLED ON PARENT_SCOPE)
			return()
		endif()
	endforeach()
endfunction()

function(add_renderdoc_test TEST_NAME TARGET_NAME KIT)
	# Fail if we can't find EnabledTests.txt
	if(NOT EXISTS ${CMAKE_SOURCE_DIR}/Labs/EnabledTests.txt)
		message(FATAL_ERROR "File ${CMAKE_SOURCE_DIR}/Labs/EnabledTests.txt not found")
	endif()

	check_test_enabled()
	if (NOT ${TEST_ENABLED})
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
	set(EXE_PATH ${PROJECT_BINARY_DIR}/${KIT})

	# renderdoccmd executable
	set(CMD_BIN renderdoccmd)

	# arguments to pass on to the executable we want to capture
	set(CMD_ARGS "capture -w -c ${KIT} ${EXE_PATH} --test --frame ${PARSED_FRAME} --framecount ${PARSED_FRAME_COUNT}")

	set(TESTDRIVER_BIN "${CMAKE_CURRENT_BINARY_DIR}/${KIT}TestDriver")

	# Probably a better way to do this.. but this works
	string (REPLACE ";" " " PARSED_UNPARSED_ARGUMENTS "${PARSED_UNPARSED_ARGUMENTS}")

	# Pass on PARSED_UNPARSED_ARGUMENTS to ignore the parsed arguments
	# we don't want to pass them to the executable
	set(TESTDRIVER_ARGS "${PARSED_UNPARSED_ARGUMENTS} --capture_file ${PROJECT_BINARY_DIR}/${KIT}_capture.rdc")


	add_test(NAME RenderDocTest COMMAND 
			${CMAKE_COMMAND}
				-DCMD1=${CMD_BIN}				# Launches test binary through renderdoccmd
				-DCMD1ARGS=${CMD_ARGS}
				-DCMD2=${TESTDRIVER_BIN}		# Executes the test driver
				-DCMD2ARGS=${TESTDRIVER_ARGS}
			-P ${CMAKE_CURRENT_SOURCE_DIR}/runtests.cmake
	)

endfunction()
