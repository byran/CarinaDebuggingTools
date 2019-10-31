#include <fstream>
#include <iostream>
#include <iomanip>
#include "Decoder.h"

#include "RawPacketDestination.h"
#include "ValuePacketValues.h"

class PacketOutput : public RawPacketDestination
{
public:
	void NonPacketBytesReceived(unsigned char* bytes, unsigned int length) override
	{
		std::cout << std::dec << "Non packet bytes " << length;
		if(length >= sizeof(BusHeader))
		{
			auto header = reinterpret_cast<BusHeader*>(bytes);
			auto totalLength = header->dataLength + sizeof(BusHeader) + sizeof(crc_t);
			std::cout << " (" << totalLength << ")";
		}
		std::cout << "\n" << std::hex << std::setfill('0');
		for(unsigned int i = 0; i < length; ++i)
		{
			std::cout << std::setw(2) << static_cast<unsigned int>(bytes[i]) << ' ';
		}
		std::cout << "\n";
	}

	void PacketReceived(BusHeader* header, unsigned int totalPacketLength) override
	{
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
		default:
			std::cout << "Packet " << static_cast<unsigned int>(header->packetType)
					  << " (" << header->dataLength << ")\n";
			break;
		}
	}

	void PacketWithInvalidCrcReceived(BusHeader* header) override
	{
		std::cout << "Packet with invalid crc\n";
	}

	void NormalSyncPacketReceived(BusSyncPacket* packet, unsigned int totalPacketLength)
	{
		unsigned int const mapMask = (1 << NUMBER_OF_BUS_TIMESLOTS) - 1;
		std::cout << "Sync packet " << std::hex << std::setw(6) << std::setfill('0')
			<< (packet->data.map & mapMask)
			<< " And: " << std::setw(8) << packet->data.andValues
			<< " Or: " << std::setw(8) << packet->data.orValues
			<< "\n";
	}

	void ValuesPacketReceived(BusValuesPacketPreamble* preamble, unsigned int totalPacketLength)
	{
		std::cout << "Number of values " << NumberOfValues(preamble) << "\n";

		for(auto&& value : preamble)
		{
			std::cout << "\t" << std::dec << std::setw(4) << std::setfill('0')
				<< (value.index & 0x7FFFu) << " => " << value.value << "\n";
		}
	}

	void TimeslotInUsePacketReceived(TimeslotInUsePacket* packet, unsigned int totalPacketLength)
	{
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