include (CMake/Modules/RenderdocTest.cmake)
include (CMake/Modules/OutputTest.cmake)
include (CMake/Modules/TestManager.cmake)

# Make sure EnabledTests.txt exists
check_enabledtests_exists()

#[[
	LabXX tests
]]
create_test_driver(LabXX)

#[[add_renderdoc_test(
		LabXX_Test_01
		LabXX						# exe
		FRAME 5						# data for test executable
		FRAME_COUNT 20
		--drawcalls_count 3			# arguments to Test Driver, the first argument here can probably be hardcoded in the function itself
)]]

add_renderdoc_test(
		LabXX_Test_02
		LabXX						
		FRAME 1
		FRAME_COUNT 1
		--vao_count 1
		--vbo_count 1
		--vbo_transfer_count 1
)

add_renderdoc_test(
		LabXX_Test_03
		LabXX						
		FRAME 1
		FRAME_COUNT 1
		--shader_count 2
)

add_renderdoc_test(
		LabXX_Test_04
		LabXX						
		FRAME 1
		FRAME_COUNT 1
		--drawcalls_count 1
)


#[[
	Lab00 tests
]]
add_output_test(Lab00 Lab00_Test_01)
