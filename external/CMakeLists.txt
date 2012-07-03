include(ExternalProject)

ExternalProject_Add(
    Eigen-3.1
    URL ${CMAKE_SOURCE_DIR}/external/eigen-eigen3_1_0-ca142d0540d3.tar.gz
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/local
    BUILD_COMMAND make install
    INSTALL_COMMAND ""
)

ExternalProject_Add(
    gtest-1.6.0
    URL ${CMAKE_SOURCE_DIR}/external/gtest-1.6.0.zip
    PREFIX ${CMAKE_SOURCE_DIR}/build
    INSTALL_COMMAND source ${CMAKE_SOURCE_DIR}/external/install_gtest.sh ${CMAKE_SOURCE_DIR}
)

ExternalProject_Add(
    cfitsio-3.3
    URL ${CMAKE_SOURCE_DIR}/external/cfitsio3300.tar.gz
    BUILD_IN_SOURCE 1
    PREFIX ${CMAKE_SOURCE_DIR}/build
    CONFIGURE_COMMAND ${CMAKE_SOURCE_DIR}/build/src/cfitsio/configure --prefix=${CMAKE_SOURCE_DIR}/local
    BUILD_COMMAND make shared
    INSTALL_COMMAND make install
)

ExternalProject_Add(
    CCfits-2.4
    DEPENDS cfitsio-3.3
    URL ${CMAKE_SOURCE_DIR}/external/CCfits-2.4.tar.gz
    BUILD_IN_SOURCE 1
    PREFIX ${CMAKE_SOURCE_DIR}/build
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/build/src/CCfits
    CONFIGURE_COMMAND <SOURCE_DIR>/configure --with-cfitsio=${CMAKE_SOURCE_DIR}/local --prefix=${CMAKE_SOURCE_DIR}/local
    BUILD_COMMAND make 
    INSTALL_COMMAND make install
)


ExternalProject_Add(
    gsl-1.15
    URL ${CMAKE_SOURCE_DIR}/external/gsl-1.15.tar.gz
    BUILD_IN_SOURCE 1
    PREFIX ${CMAKE_SOURCE_DIR}/build
    CONFIGURE_COMMAND ${CMAKE_SOURCE_DIR}/build/src/gsl/configure --prefix=${CMAKE_SOURCE_DIR}/local
    BUILD_COMMAND make 
    INSTALL_COMMAND make install
)

ExternalProject_Add(
    fftw-3.3.2
    URL ${CMAKE_SOURCE_DIR}/external/fftw-3.3.2.tar.gz
    BUILD_IN_SOURCE 1
    PREFIX ${CMAKE_SOURCE_DIR}/build
    CONFIGURE_COMMAND ${CMAKE_SOURCE_DIR}/build/src/fftw/configure --prefix=${CMAKE_SOURCE_DIR}/local --enable-mpi --enable-shared --disable-fortran
    BUILD_COMMAND make 
    INSTALL_COMMAND make install
)

ExternalProject_Add(
    boost-1.50.0
    URL ${CMAKE_SOURCE_DIR}/external/boost_1_50_0.tar.gz
    BUILD_IN_SOURCE 1
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/local/boost_1_50_0
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Add(
    petsc-3.3.p1
    URL ${CMAKE_SOURCE_DIR}/external/petsc-3.3-p1.tar.gz
    BUILD_IN_SOURCE 1
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/local/petsc-3.3-p1
    CONFIGURE_COMMAND <SOURCE_DIR>/configure --with-petsc-arch=cxx-opt --with-cc=mpicc --with-cxx=mpicxx --with-fc=0 --with-debugging=0 --with-clanguage=cxx --with-shared-libraries --with-64-bit-indices --download-f2cblaslapack=1
    BUILD_COMMAND make PETSC_DIR=<SOURCE_DIR> PETSC_ARCH=cxx-opt all
    INSTALL_COMMAND ""
)

ExternalProject_Add_Step(petsc-3.3.p1 configdebug
    WORKING_DIRECTORY <SOURCE_DIR>
    COMMAND <SOURCE_DIR>/configure --with-petsc-arch=cxx-debug --with-cc=mpicc --with-cxx=mpicxx --with-fc=0 --with-debugging=1 --with-clanguage=cxx --with-shared-libraries --with-64-bit-indices --download-f2cblaslapack=1
    DEPENDEES build
)

ExternalProject_Add_Step(petsc-3.3.p1 builddebug
    WORKING_DIRECTORY <SOURCE_DIR>
    COMMAND make PETSC_DIR=<SOURCE_DIR> PETSC_ARCH=cxx-debug all
    DEPENDEES configdebug
)

ExternalProject_Add(
    mathgl-2.0.2
    URL ${CMAKE_SOURCE_DIR}/external/mathgl-2.0.2.tar.gz
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/local
)

