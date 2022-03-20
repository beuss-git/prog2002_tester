enable_testing()

find_library(RENDERDOC_LIBRARY NAMES renderdoc REQUIRED)

include_directories(
	${CMAKE_SOURCE_DIR}/tests 
	${CMAKE_SOURCE_DIR}/external
	${CMAKE_SOURCE_DIR}/external/include
	${CMAKE_SOURCE_DIR}/external/fmt/include
)

if (MSVC)
	add_compile_definitions(RENDERDOC_PLATFORM_WIN32)
else()
	set(CMAKE_ENABLE_EXPORTS ON)
	add_compile_definitions(RENDERDOC_PLATFORM_LINUX)
endif()

# KIT = TestDriverName
# KIT_LIBS = Libraries
# KitTests = sourcefile for test(s)
function(create_test_driver KIT KIT_LIBS KitTests)
	#set(CMAKE_TESTDRIVER_BEFORE_TESTMAIN "${emscripten_before}#include \"itkTestDriverBeforeTest.inc\"")
	#set(CMAKE_TESTDRIVER_AFTER_TESTMAIN "#include \"itkTestDriverAfterTest.inc\"${emscripten_after}")

	set(ADDITIONAL_SRC ${ARGN})

	create_test_sourcelist(Tests ${KIT}TestDriver.cxx ${KitTests})

	add_executable(${KIT}TestDriver ${KIT}TestDriver.cxx ${Tests} ${ADDITIONAL_SRC} "renderdoc_helper.cpp" "renderdoc_helper.hpp")

	target_link_libraries(${KIT}TestDriver LINK_PUBLIC ${KIT_LIBS} ${RENDERDOC_LIBRARY} fmt)

	target_compile_features(${KIT}TestDriver PRIVATE cxx_std_17)
endfunction()

function(add_renderdoc_test TEST_NAME KIT)
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

	set(TESTDRIVER_BIN ${KIT}TestDriver)

	# Probably a better way to do this.. but this works
	string (REPLACE ";" " " PARSED_UNPARSED_ARGUMENTS "${PARSED_UNPARSED_ARGUMENTS}")

	# Pass on PARSED_UNPARSED_ARGUMENTS to ignore the parsed arguments
	# we don't want to pass them to the executable
	set(TESTDRIVER_ARGS "${PARSED_UNPARSED_ARGUMENTS} --capture_file ${PROJECT_BINARY_DIR}/${KIT}_capture.rdc")


	add_test(NAME ${TEST_NAME} COMMAND 
			${CMAKE_COMMAND}
				-DCMD1=${CMD_BIN}				# Launches test binary through renderdoccmd
				-DCMD1ARGS=${CMD_ARGS}
				-DCMD2=${TESTDRIVER_BIN}		# Executes the test driver
				-DCMD2ARGS=${TESTDRIVER_ARGS}
			-P ${CMAKE_CURRENT_SOURCE_DIR}/runtests.cmake
	)

endfunction()


function (add_python_renderdoc_test TEST_NAME TEST_DRIVER_ARGS COMMAND)

endfunction()
