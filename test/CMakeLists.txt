set(CMAKE_CXX_FLAGS -std=c++11)

find_package (Threads)

add_subdirectory(npio)
add_subdirectory(npgsl)
add_subdirectory(nppm)
