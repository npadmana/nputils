#executables
add_executable(liststruc liststruc.c)
target_link_libraries(liststruc cfitsio)

install(TARGETS liststruc
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
