add_executable(BusRecorderGUI
		FPSMonitor.cpp
		FPSMonitor.h
		main.cpp
		)

target_link_libraries(BusRecorderGUI PUBLIC
		CarinaBusDecoder
		sdl-cpp
		SourceCodeProFont
		Serial
		)
