add_library(npgsl SHARED npSpline.cpp npRandom.cpp)
target_link_libraries(npgsl gsl gslcblas m)

install(FILES npSpline.h npRandom.h DESTINATION include)

install(TARGETS npgsl
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
