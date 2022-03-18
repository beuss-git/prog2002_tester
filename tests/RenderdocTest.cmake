enable_testing()


include_directories(
	${CMAKE_SOURCE_DIR}/tests 
	${CMAKE_SOURCE_DIR}/external
	${CMAKE_SOURCE_DIR}/external/include
	${CMAKE_SOURCE_DIR}/external/fmt/include
)

if (MSVC)
	add_compile_definitions(RENDERDOC_PLATFORM_WIN32)
	add_compile_definitions(WIN32)
else()
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

	target_link_libraries(${KIT}TestDriver LINK_PUBLIC ${KIT_LIBS} )
	# FIXME
	target_link_libraries(${KIT}TestDriver LINK_PUBLIC "G:/repos/renderdoc/x64/Release/renderdoc.lib" fmt)


	target_compile_features(${KIT}TestDriver PRIVATE cxx_std_17)
endfunction()

function(add_renderdoc_test TEST_NAME KIT)
	add_test(${TEST_NAME} ${KIT}TestDriver ${ARGN})
endfunction()


function (add_python_renderdoc_test TEST_NAME TEST_DRIVER_ARGS COMMAND)

endfunction()
