#ifndef CARINABUSDEBUGGER_DECODEDPACKETDESTINATION_H
#define CARINABUSDEBUGGER_DECODEDPACKETDESTINATION_H

#include "Bus/Internal/Packets.h"

class DecodedPacketDestination
{
public:
	virtual ~DecodedPacketDestination() = default;
	virtual void NonPacketBytesReceived(unsigned char* bytes, unsigned int length) = 0;
	virtual void UnknownPacketReceived(BusHeader* header) = 0;
	virtual void NormalSyncPacketReceived(BusSyncPacket* packet, unsigned int totalPacketLength) = 0;
	virtual void ValuesPacketReceived(BusValuesPacketPreamble* preamble, unsigned int totalPacketLength) = 0;
	virtual void TimeslotInUsePacketReceived(TimeslotInUsePacket* packet, unsigned int totalPacketLength) = 0;
};

#endif //CARINABUSDEBUGGER_DECODEDPACKETDESTINATION_H
