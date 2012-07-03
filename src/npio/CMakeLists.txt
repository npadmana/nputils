#executables
add_executable(fits_liststruc fits_liststruc.c)
target_link_libraries(fits_liststruc cfitsio)

install(TARGETS fits_liststruc
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
