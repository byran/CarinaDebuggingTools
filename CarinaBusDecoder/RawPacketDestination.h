#ifndef CARINABUSDEBUGGER_RAWPACKETDESTINATION_H
#define CARINABUSDEBUGGER_RAWPACKETDESTINATION_H

#include "Bus/Internal/Packets.h"

class RawPacketDestination
{
public:
	virtual ~RawPacketDestination() = default;
	virtual void NonPacketBytesReceived(unsigned char* bytes, unsigned int length, uint32_t time) = 0;
	virtual void PacketReceived(BusHeader* header, unsigned int totalPacketLength, uint32_t time) = 0;
	virtual void PacketWithInvalidCrcReceived(BusHeader* header, uint32_t time) = 0;
};

#endif //CARINABUSDEBUGGER_RAWPACKETDESTINATION_H
