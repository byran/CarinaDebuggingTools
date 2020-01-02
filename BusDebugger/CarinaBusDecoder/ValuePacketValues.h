#ifndef CARINABUSDEBUGGER_VALUEPACKETVALUES_H
#define CARINABUSDEBUGGER_VALUEPACKETVALUES_H

#include "Bus/Internal/Packets.h"

unsigned int NumberOfValues(BusValuesPacketPreamble const* packetPreamble);

inline TransmittedIndexAndValue const* begin(
	BusValuesPacketPreamble const* packetPreamble)
{
	return reinterpret_cast<TransmittedIndexAndValue const*>(packetPreamble +
															 1);
}

TransmittedIndexAndValue const* end(
	BusValuesPacketPreamble const* packetPreamble);

#endif  // CARINABUSDEBUGGER_VALUEPACKETVALUES_H
