set(CMAKE_C_COMPILER "mpicc")
set(CMAKE_CXX_COMPILER "mpicxx")

include_directories(${CMAKE_SOURCE_DIR}/local/petsc-3.3-p1/include)
include_directories(${CMAKE_SOURCE_DIR}/local/petsc-3.3-p1/cxx-debug/include)
link_directories(${CMAKE_SOURCE_DIR}/local/petsc-3.3-p1/cxx-debug/lib)

add_library(nppm SHARED cpppetsc.cpp)
target_link_libraries(nppm petsc m)

install(FILES cpppetsc.h np_petsc_utils.h DESTINATION include)

install(TARGETS nppm
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

