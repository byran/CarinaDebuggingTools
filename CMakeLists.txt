cmake_minimum_required(VERSION 3.15)
project(CarinaBusDebugger)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(submodules)
add_subdirectory(Serial)
add_subdirectory(CarinaBusDecoder)
add_subdirectory(CarinaBusDebugger)
add_subdirectory(LogFileDecoder)
add_subdirectory(BusRecorder)
