#include "Decoder.h"

#include "Bus/Internal/Packets.h"

static_assert(sizeof(BusHeader) == 4, "sizeof(BusHeader) is invalid (not 4)");

unsigned char const escapeByte = 0xE0;
unsigned char const ninthBitSetByte = 0xF0;

Decoder::Decoder(RawPacketDestination* destination)
	: destination{destination}
{
}

void Decoder::DecodeByte(unsigned char byte, bool headerByte)
{
	if (headerByte && (byte == BSB_START_PACKET))
	{
		auto const lengthNotSent = bufferLength - bytesSentToDestination;
		if(lengthNotSent > 0)
		{
			destination->NonPacketBytesReceived(&buffer[bytesSentToDestination], lengthNotSent);
		}
		bufferLength = 0;
		bytesSentToDestination = 0;
		crc = 0;
	}

	if(bufferLength < bufferSize)
	{
		buffer[bufferLength] = byte;
		++bufferLength;
		crc += byte;
	}
	else
	{
		destination->NonPacketBytesReceived(&buffer[bytesSentToDestination], bufferSize - bytesSentToDestination);
		bufferLength = 0;
		bytesSentToDestination = 0;
		crc = 0;
	}

	if(bufferLength >= sizeof(BusHeader))
	{
		auto const requiredLength = header->dataLength + sizeof(BusHeader) + sizeof(crc_t);
		if(bufferLength  == requiredLength)
		{
			auto const& transmittedCrc = buffer[requiredLength - 1];
			// Remove transmitted crc as it will have been added during the loading
			// of the buffer and it isn't included in the calculated crc
			crc -= transmittedCrc;

			if(crc == transmittedCrc)
			{
				destination->PacketReceived(header, requiredLength);
			}
			else
			{
				destination->PacketWithInvalidCrcReceived(header);
			}
			bytesSentToDestination = bufferLength;
			++packetsParsed;
		}
	}
}

void Decoder::DecodeBytes(unsigned char const* bytes, unsigned int length)
{
	for(unsigned int i = 0; i < length; ++i)
	{
		unsigned char byte = bytes[i];

		if(byte == escapeByte)
		{
			inEscape = true;
		}
		else if(byte == ninthBitSetByte)
		{
			ninthBitSet = true;
		}
		else
		{
			if(inEscape)
			{
				inEscape = false;
				--byte;
			}

			DecodeByte(byte, ninthBitSet && !lastByteHadNinthBitSet);
			lastByteHadNinthBitSet = ninthBitSet;
			if(ninthBitSet)
			{
				++numberOfHeaderBytes;
				if(numberOfHeaderBytes == 4)
				{
					lastByteHadNinthBitSet = false;
					numberOfHeaderBytes = 0;
				}
			}
			else
			{
				numberOfHeaderBytes = 0;
			}
			ninthBitSet = false;
		}
	}
}
