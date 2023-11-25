# Number of testcases
set(NUM_TESTCASES 8)

# Loop for each testcase
foreach(index RANGE 1 8)
    if(4 LESS ${index})
        testcase_fail(Phase3-Test${index} ${CMAKE_CURRENT_LIST_DIR}/t${index}.txt)
    else()
        testcase_fail(Phase3-Test${index} ${CMAKE_CURRENT_LIST_DIR}/t${index}.txt)
    endif()
endforeach()
