#ifndef CARINABUSDEBUGGER_RAWPACKETDESTINATION_H
#define CARINABUSDEBUGGER_RAWPACKETDESTINATION_H

#include "Bus/Internal/Packets.h"

class RawPacketDestination
{
public:
	virtual ~RawPacketDestination() = default;
	virtual void NonPacketBytesReceived(unsigned char* bytes, unsigned int length) = 0;
	virtual void PacketReceived(BusHeader* header, unsigned int totalPacketLength) = 0;
	virtual void PacketWithInvalidCrcReceived(BusHeader* header) = 0;
};

#endif //CARINABUSDEBUGGER_RAWPACKETDESTINATION_H
