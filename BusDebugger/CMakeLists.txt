cmake_minimum_required(VERSION 3.15)
project(CarinaBusDebugger)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(submodules)
add_subdirectory(CompilerSupport)
if(${UNIX})
	add_subdirectory(Serial)
else(${UNIX})
	add_subdirectory(SerialWindows)
endif(${UNIX})
add_subdirectory(CarinaBusDecoder)
add_subdirectory(LogFileDecoder)
add_subdirectory(BusRecorder)
add_subdirectory(SourceCodeProFont)
add_subdirectory(BusRecorderGUI)
