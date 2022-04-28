

function(add_output_test TEST_TARGET TEST_NAME )
	if(MSVC) 
		set(CMD_BIN cmd)
		set(CMD_ARGS "/c ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_TARGET} > ${TEST_NAME}_out.txt")
	else()
		set(CMD_BIN sh)
		set(CMD_ARGS "-c ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_TARGET} > ${TEST_NAME}_out.txt")
	endif()

	# cmake.exe
	set(CMD2_BIN ${CMAKE_COMMAND})
	set(CMD2_ARGS "-E compare_files ${CMAKE_CURRENT_SOURCE_DIR}/data/reference/${TEST_NAME}_in.txt ${TEST_NAME}_out.txt")

	add_test(NAME ${TEST_NAME} 
		COMMAND # Run the test binary and redirect output to out.txt
			${CMAKE_COMMAND}
				-DCMD1=${CMD_BIN}
				-DCMD1ARGS=${CMD_ARGS}
				-DCMD2=${CMD2_BIN}
				-DCMD2ARGS=${CMD2_ARGS}
				-DWORKING_DIR=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
			-P ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules/RunTests.cmake
	)
endfunction()
