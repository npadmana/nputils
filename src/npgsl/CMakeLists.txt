add_library(npgsl SHARED npSpline.cpp npRandom.cpp npHistogram2D.cpp)
target_link_libraries(npgsl gsl gslcblas m)

install(FILES npSpline.h npRandom.h npHistogram2D.h DESTINATION include)

install(TARGETS npgsl
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
