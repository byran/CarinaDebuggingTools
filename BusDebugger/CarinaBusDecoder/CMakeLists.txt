if(${UNIX})
set(CarinaBusDecoderLinuxFiles
		RecordedLogFileWriter.cpp
		RecordedLogFileWriter.h
		)
endif(${UNIX})

add_library(CarinaBusDecoder
		Decoder.cpp
		Decoder.h
		RawPacketDestination.h
		RecordedLogFile.cpp
		RecordedLogFile.h
		TimestampedBytesHeader.h
		ValuePacketValues.cpp
		ValuePacketValues.h
		${CarinaBusDecoderLinuxFiles}
		)

target_link_libraries(CarinaBusDecoder PUBLIC
		Carina::Bus
		CompilerSupport
		)

target_include_directories(CarinaBusDecoder PUBLIC
		./
		)
