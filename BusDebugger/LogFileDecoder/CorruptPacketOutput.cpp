#include "CorruptPacketOutput.h"
#include "Modules/TypeR/ConvertedProject_R_Rx_IDs.h"
#include "Modules/ModuleBase/BusConfiguration.h"

#include <iomanip>
#include <iostream>
#include <sstream>

// clang-format off
char const BytesTitle[] =     "Bytes     ";
char const PacketTitle[] =    "Packet    ";
char const SyncTitle[] =      "Sync      ";
char const TimeslotTitle[] =  "TimeSlot  ";
char const ValueTitle[] =     "Value     ";
char const DismantleTitle[] = "Dismantle ";
char const InvalidTitle[] =   "Invalid   ";
// clang-format on

namespace
{
	void OutputBytes(std::ostream& stream, unsigned char* bytes, unsigned int length)
	{
		stream << std::hex << std::setfill('0');
		for (unsigned int i = 0; i < length; ++i)
		{
			stream << std::setw(2) << static_cast<unsigned int>(bytes[i])
					  << ' ';
		}
		stream << "\n";
	}
}

void CorruptPacketOutput::NonPacketBytesReceived(unsigned char* bytes, unsigned int length, uint32_t time)
{
	std::cout << lastPacket;
	lastPacket.clear();

	std::cout << BytesTitle;
	OutputBytes(std::cout, bytes, length);

	lastDataWasAnError = true;
}

void CorruptPacketOutput::PacketReceived(BusHeader* header, unsigned int totalPacketLength, uint32_t time)
{
	if (header->packetType == BPT_VALUES)
	{
		auto preamble = reinterpret_cast<BusValuesPacketPreamble*>(header);
		if(preamble->data.slot == (1 << 5))
		{
			if(preamble->data.orValues & (1 << LVOR_SYSTEM_CONFIGURATION_ALARM))
			{
				inConfigError = true;
			}
		}
		if(preamble->data.slot == (1 << 6))
		{
			std::stringstream stream;
			stream << ValueTitle;
			OutputBytes(stream, reinterpret_cast<unsigned char*>(header),
								 totalPacketLength);
			if(inConfigError)
			{
				stream << "Config Alarm\n";
			}
			if(lastDataWasAnError)
			{
				std::cout << stream.str() << "---\n";
				lastDataWasAnError = false;
			}
			else
			{
				lastPacket = stream.str();
			}
		}
	}
}

void CorruptPacketOutput::PacketWithInvalidCrcReceived(BusHeader* header, uint32_t time)
{
	std::cout << lastPacket;
	lastPacket.clear();

	std::cout << InvalidTitle;
	auto length = (header->dataLength + sizeof(BusHeader) + sizeof(crc_t));
	OutputBytes(std::cout, reinterpret_cast<unsigned char*>(header),
				length);

	lastDataWasAnError = true;
}
