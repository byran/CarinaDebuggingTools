#include "RecordedLogFile.h"
#include "TimestampedBytesHeader.h"
#include <fstream>

void DecodeRecordedLogFile(char const *filePath, TimestampedBytesDestination *destination)
{
	std::ifstream logFile(filePath, std::ios_base::binary);

	while(!logFile.eof())
	{
		char readBuffer[16384];
		TimestampedBytesHeader header;

		logFile.read(reinterpret_cast<char *>(&header), sizeof(header));
		logFile.read(readBuffer, header.length); //todo check read length

		destination->DecodeBytes(header.time, readBuffer, header.length);
	}
}