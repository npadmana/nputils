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

