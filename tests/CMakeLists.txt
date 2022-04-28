cmake_minimum_required (VERSION 3.8)
include (RenderdocTest.cmake)

# This is 'dynamic' for now, but might end up in all tests
set(Lab1Tests renderdoc_test.cpp)

create_test_driver(Lab1 "${Lab1Tests_LIBRARIES}" "${Lab1Tests}")

add_renderdoc_test(
		Lab1						# exe
		FRAME 5						# data for test executable
		FRAME_COUNT 20
		renderdoc_test				# arguments to Test Driver, the first argument here can probably be hardcoded in the function itself
		--drawcalls_count 3
)
