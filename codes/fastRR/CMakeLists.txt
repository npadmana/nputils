include_directories(${CMAKE_SOURCE_DIR}/src/npgsl)

add_executable(shellRR shellRR.cpp)
target_link_libraries(shellRR npgsl gsl gslcblas m boost_program_options)
configure_file(shellRR.cfg ${CMAKE_CURRENT_BINARY_DIR}/shellRR.cfg COPYONLY)
