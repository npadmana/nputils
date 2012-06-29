include(ExternalProject)

ExternalProject_Add(
    Eigen
    URL ${CMAKE_SOURCE_DIR}/external/eigen-eigen3_1_0-ca142d0540d3.tar.gz
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/local
    BUILD_COMMAND make install
    INSTALL_COMMAND ""
)

ExternalProject_Add(
    gtest
    URL ${CMAKE_SOURCE_DIR}/external/gtest-1.6.0.zip
    PREFIX ${CMAKE_SOURCE_DIR}/build
    INSTALL_COMMAND source ${CMAKE_SOURCE_DIR}/external/install_gtest.sh ${CMAKE_SOURCE_DIR}
)

ExternalProject_Add(
    cfitsio
    URL ${CMAKE_SOURCE_DIR}/external/cfitsio3300.tar.gz
    BUILD_IN_SOURCE 1
    PREFIX ${CMAKE_SOURCE_DIR}/build
    CONFIGURE_COMMAND ${CMAKE_SOURCE_DIR}/build/src/cfitsio/configure --prefix=${CMAKE_SOURCE_DIR}/local
    BUILD_COMMAND make shared
    INSTALL_COMMAND make install
)

