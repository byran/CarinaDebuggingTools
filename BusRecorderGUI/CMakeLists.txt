add_executable(BusRecorderGUI
		main.cpp
		)

target_link_libraries(BusRecorderGUI PUBLIC
		CarinaBusDecoder
		sdl-cpp
		SourceCodeProFont
		)
