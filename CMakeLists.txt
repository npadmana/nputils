cmake_minimum_required(VERSION 2.8)
PROJECT ( nputils )

# Build all external dependencies
add_subdirectory(external)

# Define various includes
include_directories(${CMAKE_SOURCE_DIR}/local/include)
link_directories(${CMAKE_SOURCE_DIR}/local/lib)

# Install prefix
set (CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)

# Now build all the various source pieces
#add_subdirectory(src)
