include_directories(${CMAKE_SOURCE_DIR}/src/npgsl)

add_executable(npSpline_test npSpline_test.cpp)
target_link_libraries(npSpline_test gtest gtest_main npgsl gsl gslcblas m)