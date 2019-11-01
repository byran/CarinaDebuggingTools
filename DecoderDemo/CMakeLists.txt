add_executable(DecoderDemo
		main.cpp
		)

target_link_libraries(DecoderDemo PUBLIC
		CarinaBusDecoder
		)