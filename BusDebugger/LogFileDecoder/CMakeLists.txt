add_executable(LogFileDecoder
		BasicPacketOutput.cpp
		BasicPacketOutput.h
		CorruptPacketOutput.cpp
		CorruptPacketOutput.h
		main.cpp
		)

target_link_libraries(LogFileDecoder PUBLIC
		CarinaBusDecoder
		)
