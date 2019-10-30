#include "Decoder.h"

#include "Bus/Internal/Packets.h"

static_assert(sizeof(BusHeader) == 4, "sizeof(BusHeader) is invalid (not 4)");

unsigned char const escapeByte = 0xE0;
unsigned char const ninthBitSetByte = 0xF0;

Decoder::Decoder(DecodedPacketDestination* destination)
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
	}

	if(bufferLength < bufferSize)
	{
		buffer[bufferLength] = byte;
		++bufferLength;
	}

	if(bufferLength >= sizeof(BusHeader))
	{
		auto const requiredLength = header->dataLength + sizeof(BusHeader) + sizeof(crc_t);
		if(bufferLength  == requiredLength)
		{
			switch(header->packetType)
			{
			case BPT_NORMAL_SYNC:
				destination->NormalSyncPacketReceived(reinterpret_cast<BusSyncPacket*>(buffer), requiredLength);
				break;
			case BPT_VALUES:
				destination->ValuesPacketReceived(reinterpret_cast<BusValuesPacketPreamble*>(buffer), requiredLength);;
				break;
			case BPT_TIMESLOT_IN_USE:
				destination->TimeslotInUsePacketReceived(reinterpret_cast<TimeslotInUsePacket*>(buffer), requiredLength);;
				break;
			default:
				destination->UnknownPacketReceived(header);
				break;
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
			ninthBitSet = false;
		}
	}
	//BSB_START_PACKET
}
