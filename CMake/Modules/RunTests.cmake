function(exec_check CMD ARGS)
	if(NOT DEFINED ARGS)
	  set(ARGS )
	else()
	  separate_arguments(ARGS)
	endif()

	# TODO: Fail if we failed to capture (error in we are testing)
	if(DEFINED ${WORKING_DIR})
		execute_process(COMMAND ${CMD} ${ARGS} RESULT_VARIABLE CMD_RESULT WORKING_DIRECTORY ${WORKING_DIR})
	else()
		execute_process(COMMAND ${CMD} ${ARGS} RESULT_VARIABLE CMD_RESULT)
	endif()

	if(CMD_RESULT)
        message(FATAL_ERROR "Error running ${CMD} ${ARGS}\n Error: ${CMD_RESULT}")
    endif()
endfunction()

exec_check(${CMD1} ${CMD1ARGS})
exec_check(${CMD2} ${CMD2ARGS})
