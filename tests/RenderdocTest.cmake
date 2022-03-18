enable_testing()


include_directories(${CMAKE_SOURCE_DIR}/tests)

# KIT = TestDriverName
# KIT_LIBS = Libraries
# KitTests = sourcefile for test(s)
function(create_test_driver KIT KIT_LIBS KitTests)
	#set(CMAKE_TESTDRIVER_BEFORE_TESTMAIN "${emscripten_before}#include \"itkTestDriverBeforeTest.inc\"")
	#set(CMAKE_TESTDRIVER_AFTER_TESTMAIN "#include \"itkTestDriverAfterTest.inc\"${emscripten_after}")

	set(ADDITIONAL_SRC ${ARGN})

	create_test_sourcelist(Tests ${KIT}TestDriver.cxx ${KitTests})

	add_executable(${KIT}TestDriver ${KIT}TestDriver.cxx ${Tests} ${ADDITIONAL_SRC})

	target_link_libraries(${KIT}TestDriver LINK_PUBLIC ${KIT_LIBS})
	target_compile_features(${KIT}TestDriver PRIVATE cxx_std_17)
endfunction()

function(add_renderdoc_test TEST_NAME KIT)
	add_test(${TEST_NAME} ${KIT}TestDriver ${ARGN})
endfunction()


function (add_python_renderdoc_test TEST_NAME TEST_DRIVER_ARGS COMMAND)

endfunction()
