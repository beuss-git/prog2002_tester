cmake_minimum_required (VERSION 3.8)
include (RenderdocTest.cmake)

#set(Lab1Tests test_1.cxx test_2.cxx renderdoc_test.cpp)
set(Lab1Tests renderdoc_test.cpp)

create_test_driver(Lab1 "${Lab1Tests_LIBRARIES}" "${Lab1Tests}" test_extra_stuff.cpp)

add_renderdoc_test(RenderDocTest	# test name (added for ctest)
		Lab1				# exe
		renderdoc_test				# arguments to exe
		--drawcalls_count 4
)
