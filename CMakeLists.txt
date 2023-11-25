cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(tabulate VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add directories to this project
add_subdirectory(src)
add_subdirectory(runtime)

# set up testing
enable_testing()

function(testcase_pass testname testfile)
    add_test(
        NAME ${testname}
        COMMAND ${CMAKE_BINARY_DIR}/tabulate ${testfile}
    )
    set_tests_properties(
        ${testname}
        PROPERTIES PASS_REGULAR_EXPRESSION "parsed successfully"
    )
endfunction(testcase_pass)

function(testcase_fail testname testfile)
    add_test(
        NAME ${testname}
        COMMAND ${CMAKE_BINARY_DIR}/tabulate ${testfile}
    )
    set_tests_properties(
        ${testname}
        PROPERTIES PASS_REGULAR_EXPRESSION "[0-9]*\.[0-9]*"
    )
endfunction(testcase_fail)

add_subdirectory(testcases)
