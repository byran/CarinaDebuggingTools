#include <cstring>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "Decoder.h"

#include "BasicPacketOutput.h"
using PacketOutput = BasicPacketOutput;

#include "CorruptPacketOutput.h"
//using PacketOutput = CorruptPacketOutput;

char HexCharToValue(char c)
{
	if(c >= 'a' && c <= 'f')
	{
		return 10 + (c - 'a');
	}

	if(c >= 'A' && c <= 'F')
	{
		return 10 + (c - 'A');
	}

	return c - '0';
}

int ParseAsciiFile(char* fileName)
{
	PacketOutput output;
	Decoder decoder{&output};
	std::ifstream file{fileName, std::ios::in};

	while(!file.eof())
	{
		char buffer[1024];
		uint32_t length{0};
		while((length < sizeof(buffer)) && !file.eof())
		{
			char b[2];
			file.read(b, 2);
			buffer[length] = (HexCharToValue(b[0]) << 4) | HexCharToValue(b[1]);
			++length;
		}
		decoder.DecodeBytes(reinterpret_cast<unsigned char*>(buffer),
							length);
	}

	std::cout << std::dec << "Packets parsed: " << decoder.packetsParsed
			  << std::endl;
	return 0;
}

int ParseNonTimestampedFile(char* fileName)
{
	PacketOutput output;
	Decoder decoder{&output};
	std::ifstream file{fileName, std::ios::binary};

	char buffer[1024];
	decltype(file.tellg()) readLength;
	auto previousPosition = file.tellg();
	do
	{
		file.read(buffer, sizeof(buffer));
		auto position = file.tellg();
		readLength = position - previousPosition;
		previousPosition = position;
		decoder.DecodeBytes(reinterpret_cast<unsigned char*>(buffer),
							readLength);
	} while (readLength > 0);

	std::cout << std::dec << "Packets parsed: " << decoder.packetsParsed
			  << std::endl;
	return 0;
}

int ParseTimestampedFile(char* fileName)
{
	PacketOutput output;
	Decoder decoder{&output};
	DecodeRecordedLogFile(fileName, &decoder);
	return 0;
}

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		std::cerr << "No file specified\n";
		return 1;
	}

	if (strcmp(argv[1], "old") == 0)
	{
		return ParseNonTimestampedFile(argv[2]);
	}

	if (strcmp(argv[1], "timestamped") == 0)
	{
		return ParseTimestampedFile(argv[2]);
	}

	if (strcmp(argv[1], "ascii") == 0)
	{
		return ParseAsciiFile(argv[2]);
	}

	return 2;
}
