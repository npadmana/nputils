include_directories(${CMAKE_SOURCE_DIR}/src/npgsl)

set (testlist npSpline_test)

foreach (test1 ${testlist})
add_executable(${test1} ${test1}.cpp)
target_link_libraries(${test1} gtest gtest_main npgsl gsl gslcblas m ${CMAKE_THREAD_LIBS_INIT})
endforeach(test1)

install(TARGETS ${testlist}
    RUNTIME DESTINATION testbin/npgsl
)
