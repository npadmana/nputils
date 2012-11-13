cmake_minimum_required(VERSION 2.8)
PROJECT ( nputils )

# Build all external dependencies
add_subdirectory(external)

# Define various includes
include_directories(${CMAKE_SOURCE_DIR}/local/include)
include_directories(${CMAKE_SOURCE_DIR}/local/include/eigen3)
#include_directories(${CMAKE_SOURCE_DIR}/install/include)
link_directories(${CMAKE_SOURCE_DIR}/local/lib)
#link_directories(${CMAKE_SOURCE_DIR}/install/lib)

# Install prefix
set (CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)

# Now build all the various source pieces
add_subdirectory(src)
add_subdirectory(test)

# add in codes
add_subdirectory(codes)


#------------------------------------------------
# Make this generally available for outside codes
export(PACKAGE nputils)

export(TARGETS npgsl  FILE "${PROJECT_BINARY_DIR}/nputilsLibraryDepends.cmake")

# Create a nputilsBuildTreeSettings.cmake file for the use from the build tree
configure_file(nputilsBuildTreeSettings.cmake.in
  "${PROJECT_BINARY_DIR}/nputilsBuildTreeSettings.cmake" @ONLY)

configure_file(nputilsConfig.cmake.in
  "${PROJECT_BINARY_DIR}/nputilsConfig.cmake" @ONLY)  
  