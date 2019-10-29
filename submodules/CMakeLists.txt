include_directories(Carina/cpp/)

add_subdirectory(Carina/cpp/Bootloader)
add_subdirectory(Carina/cpp/Common)
add_subdirectory(Carina/cpp/Bus)
add_subdirectory(sdl_cpp_wrapper)

target_include_directories(Bus PUBLIC Carina/cpp/)
