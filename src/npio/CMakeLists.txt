#executables
add_executable(fits_liststruc fits_liststruc.c)
target_link_libraries(fits_liststruc cfitsio m)

add_executable(fits_listhead fits_liststruc.c)
target_link_libraries(fits_listhead cfitsio m)

add_executable(fits_copy fits_liststruc.c)
target_link_libraries(fits_copy cfitsio m)


install(TARGETS fits_liststruc fits_listhead fits_copy
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
