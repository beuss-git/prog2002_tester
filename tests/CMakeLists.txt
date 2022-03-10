cmake_minimum_required (VERSION 3.8)
include (RenderdocTest.cmake)

#set(Extra_SRCS test_extra_stuff.cpp)
#include_directories(${CMAKE_SOURCE_DIR}/tests)
#create_test_sourcelist(testSrcs
						#TestDriverTest.cxx
						#test_1.cxx
						#test_2.cxx
						#EXTRA_INCLUDE test_args.h 
						#FUNCTION testProccessArgs)
set(Lab1Tests test_1.cxx test_2.cxx)
create_test_driver(Lab1 "${Lab1Tests_LIBRARIES}" "${Lab1Tests}" test_extra_stuff.cpp)

#add_executable(TestDriverTest ${testSrcs} ${Extra_SRCS})


add_renderdoc_test(Test1	# test name (added for ctest)
		Lab1				# exe
		test_2				# arguments to exe
)

#add_test(Test1 Tes test_2)
#set(TTT Test1 Tes test_2)
#add_test(${TTT})
