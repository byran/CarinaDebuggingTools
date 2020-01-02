add_executable(LogFileDecoder
		main.cpp
		)

target_link_libraries(LogFileDecoder PUBLIC
		CarinaBusDecoder
		)