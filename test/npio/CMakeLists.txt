#executables
add_executable(fits_make_test_bintable fits_make_test_bintable.cpp)
target_link_libraries(fits_make_test_bintable CCfits cfitsio m)


set(datafiles inputtextfile_comment.txt inputtextfile_nocomment.txt inputtextfile_commentgzip.txt.gz)
foreach(data1 ${datafiles})
configure_file(${data1} ${CMAKE_CURRENT_BINARY_DIR}/${data1} COPYONLY)
endforeach(data1)