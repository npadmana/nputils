set(CMAKE_C_COMPILER "mpicc")
set(CMAKE_CXX_COMPILER "mpicxx")
set(CMAKE_CXX_FLAGS -std=c++11)


include_directories(${CMAKE_SOURCE_DIR}/src/npgsl)
include_directories(${CMAKE_SOURCE_DIR}/src/nppm)
include_directories(${CMAKE_SOURCE_DIR}/local/petsc-3.3-p1/include)
include_directories(${CMAKE_SOURCE_DIR}/local/petsc-3.3-p1/cxx-debug/include)
link_directories(${CMAKE_SOURCE_DIR}/local/petsc-3.3-p1/cxx-debug/lib)

set (testlist cpppetsc_test Particles_test)


foreach (test1 ${testlist})
add_executable(${test1} ${test1}.cpp)
target_link_libraries(${test1} gtest nppm petsc npgsl m)
endforeach(test1)

install(TARGETS ${testlist}
    RUNTIME DESTINATION testbin/nppm
)
