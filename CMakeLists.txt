cmake_minimum_required(VERSION 2.8)
PROJECT ( nputils )

# Build all external dependencies
add_subdirectory(external)

# Define various includes
include_directories(${CMAKE_SOURCE_DIR}/local/include)
include_directories(${CMAKE_SOURCE_DIR}/local/boost_1_50_0)
include_directories(${CMAKE_SOURCE_DIR}/local/include/eigen3)
#include_directories(${CMAKE_SOURCE_DIR}/install/include)
link_directories(${CMAKE_SOURCE_DIR}/local/lib)
#link_directories(${CMAKE_SOURCE_DIR}/install/lib)

# Install prefix
set (CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)

# Now build all the various source pieces
add_subdirectory(src)
add_subdirectory(test)
