#ifndef CARINABUSDEBUGGER_DECODER_H
#define CARINABUSDEBUGGER_DECODER_H

#include "DecodedPacketDestination.h"

struct BusHeader;

class Decoder
{
private:
	static unsigned int const bufferSize = 1024;
	unsigned char buffer[bufferSize];
	unsigned int bufferLength{0};
	bool inEscape{false};
	bool ninthBitSet{false};
	bool lastByteHadNinthBitSet{false};

	BusHeader* header{reinterpret_cast<BusHeader*>(buffer)};

	unsigned int bytesSentToDestination{0};
	DecodedPacketDestination* const destination;
public:
	Decoder(DecodedPacketDestination* destination);
private:
	void DecodeByte(unsigned char byte, bool headerByte);
public:
	void DecodeBytes(unsigned char const* bytes, unsigned int length);
	unsigned int packetsParsed{0};
};

#endif //CARINABUSDEBUGGER_DECODER_H
