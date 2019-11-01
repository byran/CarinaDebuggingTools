#include "RecordedLogFile.h"
#include <fstream>

struct blockHeader
{
	uint32_t	time;
	uint32_t	length;
} __attribute__((packed));

void DecodeRecordedLogFile(char const *filePath, TimestampedBytesDestination *destination)
{
	std::ifstream logFile(filePath, std::ios_base::binary);

	while(!logFile.eof())
	{
		char readBuffer[16384];
		blockHeader header;

		logFile.read(reinterpret_cast<char *>(&header), sizeof(header));
		logFile.read(readBuffer, header.length); //todo check read length

		destination->DecodeBytes(header.time, readBuffer, header.length);
	}
}