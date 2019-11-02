add_library(CarinaBusDecoder
		Decoder.cpp
		Decoder.h
		RawPacketDestination.h
		RecordedLogFile.cpp
		RecordedLogFile.h
		TimestampedBytesHeader.h
		ValuePacketValues.cpp
		ValuePacketValues.h
		)

target_link_libraries(CarinaBusDecoder PUBLIC
		Carina::Bus
		)

target_include_directories(CarinaBusDecoder PUBLIC
		./
		)