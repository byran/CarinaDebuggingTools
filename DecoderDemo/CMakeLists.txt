add_executable(DecoderDemo
		main.cpp
		SerialPort.cpp
		SerialPort.h
		)

target_link_libraries(DecoderDemo PUBLIC
		CarinaBusDecoder
		)