#ifndef CARINABUSDEBUGGER_BASICPACKETOUTPUT_H
#define CARINABUSDEBUGGER_BASICPACKETOUTPUT_H

#include "RawPacketDestination.h"

class BasicPacketOutput : public RawPacketDestination
{
public:
	unsigned int packetCount{0};

	void OutputTimeAndPacketCount(uint32_t time) const;
	void NonPacketBytesReceived(unsigned char* bytes, unsigned int length,
								uint32_t time) override;
	void PacketReceived(BusHeader* header, unsigned int totalPacketLength,
						uint32_t time) override;

	void PacketWithInvalidCrcReceived(BusHeader* header, uint32_t time) override;
};


#endif	// CARINABUSDEBUGGER_BASICPACKETOUTPUT_H
