set(CMAKE_CXX_FLAGS -std=c++11)

#executables
add_executable(fits_make_test_bintable fits_make_test_bintable.cpp)
target_link_libraries(fits_make_test_bintable CCfits cfitsio m)

include_directories(${CMAKE_SOURCE_DIR}/src/npio)
set (testlist npTextFile_test)

foreach (test1 ${testlist})
add_executable(${test1} ${test1}.cpp)
target_link_libraries(${test1} gtest gtest_main npio)
endforeach(test1)





set(datafiles inputtextfile_comment.txt inputtextfile_nocomment.txt inputtextfile_commentgzip.txt.gz)
foreach(data1 ${datafiles})
configure_file(${data1} ${CMAKE_CURRENT_BINARY_DIR}/${data1} COPYONLY)
endforeach(data1)