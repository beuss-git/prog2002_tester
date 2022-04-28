include(CMake/Modules/TestManager.cmake)

function(add_output_test TEST_TARGET TEST_NAME )
	check_test_enabled()
	if (NOT ${TEST_ENABLED})
		message(WARNING "Test ${TEST_NAME} is not enabled in EnabledTests.txt")
		return()
	endif()

	add_test(NAME ${TEST_NAME}
			COMMAND ${CMAKE_COMMAND}
			-DCOMMAND=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_TARGET}
			-DOUTPUT=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME}_out.txt
			-DCOMPARE=${CMAKE_CURRENT_SOURCE_DIR}/data/reference/${TEST_NAME}_in.txt
			-DIGNORE_LINE_ENDINGS=ON
			-P ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules/RunAndCompare.cmake
			)
endfunction()
