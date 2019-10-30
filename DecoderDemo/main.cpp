#include <fstream>
#include <iostream>
#include <iomanip>
#include "Decoder.h"

#include "DecodedPacketDestination.h"

class PacketOutput : public DecodedPacketDestination
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

	void UnknownPacketReceived(BusHeader* header) override
	{
		std::cout << "Packet " << static_cast<unsigned int>(header->packetType)
		<< " (" << header->dataLength << ")\n";
	}

	void NormalSyncPacketReceived(BusSyncPacket* packet, unsigned int totalPacketLength) override
	{
		unsigned int const mapMask = (1 << NUMBER_OF_BUS_TIMESLOTS) - 1;
		std::cout << "Sync packet " << std::hex << std::setw(6) << std::setfill('0')
			<< (packet->data.map & mapMask)
			<< " And: " << std::setw(8) << packet->data.andValues
			<< " Or: " << std::setw(8) << packet->data.orValues
			<< "\n";
	}

	void ValuesPacketReceived(BusValuesPacketPreamble* preamble, unsigned int totalPacketLength) override
	{
		unsigned int sizeofValues = totalPacketLength - (sizeof(BusValuesPacketPreamble) + sizeof(crc_t));
		unsigned int numberOfValues = sizeofValues / sizeof(IndexAndValue);
		auto values = reinterpret_cast<IndexAndValue*>(&preamble[1]);
		std::cout << "Number of values " << numberOfValues << "\n";
	}

	void TimeslotInUsePacketReceived(TimeslotInUsePacket* packet, unsigned int totalPacketLength) override
	{
	}
};

int main(int argc, char** argv)
{
	PacketOutput output;
	Decoder decoder{&output};

	std::ifstream file{"/home/byran/bus_log.bin"};

	char buffer[1024];
	unsigned int readLength;
	do
	{
		readLength = file.readsome(buffer, sizeof(buffer));
		decoder.DecodeBytes(reinterpret_cast<unsigned char*>(buffer), readLength);
	} while(readLength > 0);

	std::cout << std::dec << "Packets parsed: " << decoder.packetsParsed << std::endl;
}