set(CMAKE_C_COMPILER "mpicc")
set(CMAKE_CXX_COMPILER "mpicxx")

include_directories(${CMAKE_SOURCE_DIR}/src/nppm)
include_directories(${CMAKE_SOURCE_DIR}/local/petsc-3.3-p1/include)
include_directories(${CMAKE_SOURCE_DIR}/local/petsc-3.3-p1/cxx-debug/include)
link_directories(${CMAKE_SOURCE_DIR}/local/petsc-3.3-p1/cxx-debug/lib)

add_executable(cpppetsc_test cpppetsc_test.cpp)
target_link_libraries(cpppetsc_test gtest petsc m)
