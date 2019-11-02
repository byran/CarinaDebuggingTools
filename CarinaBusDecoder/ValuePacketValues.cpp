#include "ValuePacketValues.h"

static_assert(sizeof(IndexAndValue) == 8,
			  "sizeof(IndexAndValue) is invalid (not IndexAndValue)");

unsigned int NumberOfValues(BusValuesPacketPreamble const* packetPreamble)
{
	unsigned int const totalPacketLength =
		packetPreamble->header.dataLength + sizeof(BusHeader) + sizeof(crc_t);
	unsigned int const sizeofValues =
		totalPacketLength - (sizeof(BusValuesPacketPreamble) + sizeof(crc_t));
	return sizeofValues / sizeof(TransmittedIndexAndValue);
}

TransmittedIndexAndValue const* end(
	BusValuesPacketPreamble const* packetPreamble)
{
	return begin(packetPreamble) + NumberOfValues(packetPreamble);
}
