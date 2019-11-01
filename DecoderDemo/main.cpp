#include <fstream>
#include <iostream>
#include <iomanip>
#include "Decoder.h"

#include "RawPacketDestination.h"
#include "ValuePacketValues.h"

char const BytesTitle[] =      "Bytes     ";
char const PacketTitle[] =     "Packet    ";
char const SyncTitle[] =       "Sync      ";
char const TimeslotTitle[] =   "TimeSlot  ";
char const ValueTitle[] =      "Value     ";
char const DismantleTitle[] =  "Dismantle ";

class PacketOutput : public RawPacketDestination
{
public:
	unsigned int packetCount{0};

	void OutputTimeAndPacketCount(uint32_t time)
	{
		std::cout << std::dec << std::setfill('0') << std::setw(8) << time << " | " << std::setw(8) << packetCount << ": ";
	}

	void NonPacketBytesReceived(unsigned char* bytes, unsigned int length, uint32_t time) override
	{
		OutputTimeAndPacketCount(time);

		std::cout << BytesTitle << std::dec << length;
		if(length >= sizeof(BusHeader))
		{
			auto header = reinterpret_cast<BusHeader*>(bytes);
			auto totalLength = header->dataLength + sizeof(BusHeader) + sizeof(crc_t);
			std::cout << " (" << totalLength << ")";
		}
		std::cout << " " << std::hex << std::setfill('0');
		for(unsigned int i = 0; i < length; ++i)
		{
			std::cout << std::setw(2) << static_cast<unsigned int>(bytes[i]) << ' ';
		}
		std::cout << "\n";
	}

	void PacketReceived(BusHeader* header, unsigned int totalPacketLength, uint32_t time) override
	{
		OutputTimeAndPacketCount(time);
		++packetCount;

		switch (header->packetType)
		{
		case BPT_NORMAL_SYNC:
			NormalSyncPacketReceived(reinterpret_cast<BusSyncPacket*>(header), totalPacketLength);
			break;
		case BPT_VALUES:
			ValuesPacketReceived(reinterpret_cast<BusValuesPacketPreamble*>(header),
								 totalPacketLength);
			break;
		case BPT_TIMESLOT_IN_USE:
			TimeslotInUsePacketReceived(reinterpret_cast<TimeslotInUsePacket*>(header),
										totalPacketLength);
			break;
		case BPT_DISMANTLE_BUS:
			DismantlePacketReceived(reinterpret_cast<BusDismantlePacket*>(header),
										totalPacketLength);
			break;
		default:
			std::cout << PacketTitle << static_cast<unsigned int>(header->packetType)
					  << " (" << header->dataLength << ")\n";
			break;
		}
	}

	void PacketWithInvalidCrcReceived(BusHeader* header, uint32_t time) override
	{
		OutputTimeAndPacketCount(time);

		std::cout << "Packet with invalid crc\n";
		auto buffer = reinterpret_cast<unsigned char*>(header);
		for(uint32_t i = 0; i < (header->dataLength + sizeof(BusHeader)); ++i)
		{
			std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<uint32_t>(buffer[i]) << " ";
		}
		std::cout << "\n";
	}

	uint32_t previousMap;
	void NormalSyncPacketReceived(BusSyncPacket* packet, unsigned int totalPacketLength)
	{
		unsigned int const mapMask = (1 << NUMBER_OF_BUS_TIMESLOTS) - 1;
		uint32_t const map = (packet->data.map & mapMask);

		if(map != previousMap)
		{
			std::cout << "Map Changed ";
		}
		previousMap = map;

		std::cout << SyncTitle << std::hex << std::setw(6) << std::setfill('0')
			<< map
			<< " And: " << std::setw(8) << packet->data.andValues
			<< " Or: " << std::setw(8) << packet->data.orValues
			<< "\n";
	}

	void ValuesPacketReceived(BusValuesPacketPreamble* preamble, unsigned int totalPacketLength)
	{
		std::cout << ValueTitle << std::dec << preamble->data.slot << "\n";
		uint16_t required = 291 - 177;
		uint16_t requiredUpper = required + 4;
		//std::cout << "Number of values " << NumberOfValues(preamble) << "\n";

		for(auto&& value : preamble)
		{
			auto const index = (value.index & 0x7FFFu);
			if(index >= required && index < requiredUpper)
			{
				std::cout << std::dec << std::setw(4) << std::setfill('0')
					<< (value.index & 0x7FFFu) << " => " << value.value << "\n";
			}
		}
	}

	void TimeslotInUsePacketReceived(TimeslotInUsePacket* packet, unsigned int totalPacketLength)
	{
		std::cout << TimeslotTitle << packet->data.slot << "\n";
	}

	void DismantlePacketReceived(BusDismantlePacket* packet, unsigned int totalPacketLength)
	{
		std::cout << DismantleTitle << "\n";
	}
};

int main(int argc, char** argv)
{
	PacketOutput output;
	Decoder decoder{&output};

	if(argc < 2)
	{
		std::cerr << "No file specified\n";
		return 1;
	}
	std::ifstream file{argv[1]};

	char buffer[1024];
	unsigned int readLength;
	do
	{
		readLength = file.readsome(buffer, sizeof(buffer));
		decoder.DecodeBytes(reinterpret_cast<unsigned char*>(buffer), readLength);
	} while(readLength > 0);

	std::cout << std::dec << "Packets parsed: " << decoder.packetsParsed << std::endl;
}