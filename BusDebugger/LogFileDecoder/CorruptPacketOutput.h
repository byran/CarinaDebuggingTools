#ifndef CARINABUSDEBUGGER_CORRUPTPACKETOUTPUT_H
#define CARINABUSDEBUGGER_CORRUPTPACKETOUTPUT_H

#include "RawPacketDestination.h"

#include <string>

class CorruptPacketOutput : public RawPacketDestination
{
private:
	std::string lastPacket;
	bool lastDataWasAnError{false};
	bool inConfigError{false};

public:
	void NonPacketBytesReceived(unsigned char* bytes, unsigned int length,
								uint32_t time) override;

	void PacketReceived(BusHeader* header, unsigned int totalPacketLength,
						uint32_t time) override;

	void PacketWithInvalidCrcReceived(BusHeader* header, uint32_t time) override;
};

#endif	// CARINABUSDEBUGGER_CORRUPTPACKETOUTPUT_H
