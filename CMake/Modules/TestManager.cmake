
function(check_enabledtests_exists)
	# Fail if we can't find EnabledTests.txt
	if(NOT EXISTS ${CMAKE_SOURCE_DIR}/Labs/EnabledTests.txt)
		message(FATAL_ERROR "File ${CMAKE_SOURCE_DIR}/Labs/EnabledTests.txt not found")
	endif()
endfunction()

# Sets TEST_ENABLED if match is found in EnabledTests.txt
function(check_test_enabled)
	# Read in the file
	file(READ ${CMAKE_SOURCE_DIR}/Labs/EnabledTests.txt TESTS_INPUT)

	set(TEST_ENABLED OFF PARENT_SCOPE)

	# I have to match line by line because ^ is broken when reading the file like this
	STRING(REGEX REPLACE "\r?\n" ";" TESTS_INPUT "${TESTS_INPUT}")
	foreach(line ${TESTS_INPUT})
		if("${line}" MATCHES "^[\\s]*${TEST_NAME}$")
			set(TEST_ENABLED ON PARENT_SCOPE)
			return()
		endif()
	endforeach()
endfunction()
