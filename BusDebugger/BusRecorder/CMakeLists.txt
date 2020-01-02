add_executable(BusRecorder
		main.cpp
		)

target_link_libraries(BusRecorder
		Serial
		CarinaBusDecoder
		)