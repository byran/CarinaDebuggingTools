#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "Decoder.h"

#include "RawPacketDestination.h"
#include "ValuePacketValues.h"
#include "Modules/TypeR/ConvertedProject_R_Rx_IDs.h"

// clang-format off
char const BytesTitle[] =     "Bytes     ";
char const PacketTitle[] =    "Packet    ";
char const SyncTitle[] =      "Sync      ";
char const TimeslotTitle[] =  "TimeSlot  ";
char const ValueTitle[] =     "Value     ";
char const DismantleTitle[] = "Dismantle ";
// clang-format on

class PacketOutput : public RawPacketDestination
{
public:
	unsigned int packetCount{0};

	void OutputTimeAndPacketCount(uint32_t time)
	{
		std::cout << std::dec << std::setfill('0') << std::setw(8) << time
				  << " | " << std::setw(8) << packetCount << ": ";
	}

	void NonPacketBytesReceived(unsigned char* bytes, unsigned int length)
	{
		std::cout << BytesTitle << std::dec << length;
		if (length >= sizeof(BusHeader))
		{
			auto header = reinterpret_cast<BusHeader*>(bytes);
			auto totalLength =
				header->dataLength + sizeof(BusHeader) + sizeof(crc_t);
			std::cout << " (" << totalLength << ")";
		}
		std::cout << " " << std::hex << std::setfill('0');
		for (unsigned int i = 0; i < length; ++i)
		{
			std::cout << std::setw(2) << static_cast<unsigned int>(bytes[i])
					  << ' ';
		}
		std::cout << "\n";
	}

	void NonPacketBytesReceived(unsigned char* bytes, unsigned int length,
								uint32_t time) override
	{
		OutputTimeAndPacketCount(time);
		NonPacketBytesReceived(bytes, length);
	}

	void PacketReceived(BusHeader* header, unsigned int totalPacketLength,
						uint32_t time) override
	{
		OutputTimeAndPacketCount(time);
		++packetCount;

		switch (header->packetType)
		{
		case BPT_NORMAL_SYNC:
			NormalSyncPacketReceived(reinterpret_cast<BusSyncPacket*>(header),
									 totalPacketLength);
			break;
		case BPT_VALUES:
			ValuesPacketReceived(
				reinterpret_cast<BusValuesPacketPreamble*>(header),
				totalPacketLength);
			break;
		case BPT_TIMESLOT_IN_USE:
			TimeslotInUsePacketReceived(
				reinterpret_cast<TimeslotInUsePacket*>(header),
				totalPacketLength);
			break;
		case BPT_DISMANTLE_BUS:
			DismantlePacketReceived(
				reinterpret_cast<BusDismantlePacket*>(header),
				totalPacketLength);
			break;
		default:
			std::cout << PacketTitle
					  << static_cast<unsigned int>(header->packetType) << " ("
					  << header->dataLength << ")\n";
			break;
		}
	}

	void PacketWithInvalidCrcReceived(BusHeader* header, uint32_t time) override
	{
		OutputTimeAndPacketCount(time);

		std::cout << "Packet with invalid crc\n";
		auto buffer = reinterpret_cast<unsigned char*>(header);
		for (uint32_t i = 0;
			 i < (header->dataLength + sizeof(BusHeader) + sizeof(crc_t)); ++i)
		{
			std::cout << std::hex << std::setw(2) << std::setfill('0')
					  << static_cast<uint32_t>(buffer[i]) << " ";
		}
		std::cout << "\n";
	}

	void OutputMap(uint32_t map)
	{
		auto AddModuleMapText = [map](uint32_t index, char const* moduleText) {
			if (map & (1 << index))
			{
				std::cout << moduleText;
			}
			else
			{
				std::cout << "...";
			}
		};

		AddModuleMapText(0, "R--");
		AddModuleMapText(1, "A1-");
		AddModuleMapText(2, "A2-");
		AddModuleMapText(3, "A3-");
		AddModuleMapText(4, "A4-");
		AddModuleMapText(5, "B--");
		AddModuleMapText(6, "D1-");
		AddModuleMapText(7, "D2-");
		AddModuleMapText(8, "D3-");
		AddModuleMapText(9, "D4-");
		AddModuleMapText(10, "*--");
		AddModuleMapText(11, "*--");
		AddModuleMapText(12, "*--");
		AddModuleMapText(13, "*--");
		AddModuleMapText(14, "*--");
		AddModuleMapText(15, "*--");
		AddModuleMapText(16, "*--");
		AddModuleMapText(17, "*--");
		AddModuleMapText(18, "*--");
		AddModuleMapText(19, "Q--");
	}

	uint32_t previousMap;
	void NormalSyncPacketReceived(BusSyncPacket* packet,
								  unsigned int totalPacketLength)
	{
		if (totalPacketLength != sizeof(BusSyncPacket))
		{
			NonPacketBytesReceived(reinterpret_cast<unsigned char*>(packet),
								   totalPacketLength);
			return;
		}
		unsigned int const mapMask = (1 << NUMBER_OF_BUS_TIMESLOTS) - 1;
		uint32_t const map = (packet->data.map & mapMask);

		std::cout << SyncTitle;
		OutputMap(map);
		std::cout << " " << std::hex << std::setw(6) << std::setfill('0')
				  << map;

		if (map != previousMap)
		{
			std::cout << " Map Changed ";
		}
		previousMap = map;

		std::cout << " And: " << std::setw(8) << packet->data.andValues
				  << " Or: " << std::setw(8) << packet->data.orValues << "\n";
	}

	void ValuesPacketReceived(BusValuesPacketPreamble* preamble,
							  unsigned int totalPacketLength)
	{
		std::cout << ValueTitle;  // << std::dec << preamble->data.slot << "\n";
		OutputMap(preamble->data.slot);
		std::cout << "\n";
		// std::cout << "Number of values " << NumberOfValues(preamble) << "\n";

		for (auto&& value : preamble)
		{
			auto const index = (value.index & 0x7FFFu);
			switch(index)
			{
			case BUS_VARIANTS_R:
			case BUS_VARIANTS_A:
			case BUS_VARIANTS_B:
			case BUS_VARIANTS_D:
			case BUS_VARIANTS_K:
			case BUS_VARIANTS_E:
			case BUS_VARIANTS_Q:
			case BUS_VARIANTS_F:
				std::cout << std::dec << std::setw(4) << std::setfill('0')
						  << (value.index & 0x7FFFu) << " => " << value.value
						  << "\n";
				break;
			default:
				break;
			}
		}
	}

	void TimeslotInUsePacketReceived(TimeslotInUsePacket* packet,
									 unsigned int totalPacketLength)
	{
		std::cout << TimeslotTitle << packet->data.slot << "\n";
	}

	void DismantlePacketReceived(BusDismantlePacket* packet,
								 unsigned int totalPacketLength)
	{
		std::cout << DismantleTitle << "\n";
	}
};

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

	return 2;
}
