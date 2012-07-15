add_library(npgsl SHARED npSpline.cpp)
target_link_libraries(npgsl gsl gslcblas m)