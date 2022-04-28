include (CMake/Modules/RenderdocTest.cmake)



#[[
	LabXX tests
]]
create_test_driver(LabXX)

add_renderdoc_test(
		LabXX_Test_01	
		LabXX						# exe
		FRAME 5						# data for test executable
		FRAME_COUNT 20
		--drawcalls_count 3			# arguments to Test Driver, the first argument here can probably be hardcoded in the function itself
)


#[[
	Lab00 tests
]]
set(CMD_BIN cmd)
set(CMD_ARGS "/c ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Lab00 > Lab00_test_01_out.txt")

set(CMD2_BIN ${CMAKE_COMMAND})
set(CMD2_ARGS "-E compare_files ${CMAKE_CURRENT_SOURCE_DIR}/data/reference/Lab00_Test_01.txt Lab00_test_01_out.txt")

add_test(NAME lab00test 
	COMMAND # Run the test binary and redirect output to out.txt
		${CMAKE_COMMAND}
			-DCMD1=${CMD_BIN}
			-DCMD1ARGS=${CMD_ARGS}
			-DCMD2=${CMD2_BIN}
			-DCMD2ARGS=${CMD2_ARGS}
			-DWORKING_DIR=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		-P ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules/RunTests.cmake
)
