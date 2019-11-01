#ifndef CARINABUSDEBUGGER_DECODER_H
#define CARINABUSDEBUGGER_DECODER_H

#include "RawPacketDestination.h"

struct BusHeader;

class Decoder
{
private:
	static unsigned int const bufferSize = 1024;
	unsigned char buffer[bufferSize];
	unsigned int bufferLength{0};
	crc_t crc{0};
	bool inEscape{false};
	bool ninthBitSet{false};
	bool lastByteHadNinthBitSet{false};
	unsigned int numberOfHeaderBytes{0};

	BusHeader* header{reinterpret_cast<BusHeader*>(buffer)};

	RawPacketDestination* const destination;
public:
	Decoder(RawPacketDestination* destination);
private:
	void DecodeByte(unsigned char byte, bool headerByte);
public:
	void DecodeBytes(unsigned char const* bytes, unsigned int length);
	unsigned int packetsParsed{0};
};

#endif //CARINABUSDEBUGGER_DECODER_H
