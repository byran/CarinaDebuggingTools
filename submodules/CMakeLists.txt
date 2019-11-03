include_directories(Carina/cpp/)

add_compile_definitions(WIN32)

add_subdirectory(Carina/cpp/Bootloader)
add_subdirectory(Carina/cpp/Common)
add_subdirectory(Carina/cpp/Runner)
add_subdirectory(Carina/cpp/Bus)

add_subdirectory(Carina/cpp/Unity)

add_subdirectory(Carina/cpp/PICDriversInterfaces)
add_subdirectory(Carina/cpp/DeviceDrivers)

add_subdirectory(Carina/cpp/TestDoubles/CommonDoubles)
add_subdirectory(Carina/cpp/TestDoubles/DeviceDriversDoubles)
add_subdirectory(Carina/cpp/TestDoubles/BusDoubles)

add_subdirectory(Carina/cpp/Bus.Tests)


add_subdirectory(sdl_cpp_wrapper)

target_include_directories(Bus PUBLIC Carina/cpp/)
