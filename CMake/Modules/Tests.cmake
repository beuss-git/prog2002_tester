include (CMake/Modules/RenderdocTest.cmake)


create_test_driver(LabXX)

add_renderdoc_test(
		LabXX_Test_01	
		LabXX						# exe
		FRAME 5						# data for test executable
		FRAME_COUNT 20
		--drawcalls_count 3			# arguments to Test Driver, the first argument here can probably be hardcoded in the function itself
)