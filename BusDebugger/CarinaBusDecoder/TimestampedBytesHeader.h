#ifndef CARINABUSDEBUGGER_TIMESTAMPEDBYTESHEADER_H
#define CARINABUSDEBUGGER_TIMESTAMPEDBYTESHEADER_H

#include <cstdint>
#include "CompilerSupport.h"

#ifdef WIN32
#pragma pack(1)
#endif

struct TimestampedBytesHeader
{
	uint32_t time;
	uint32_t length;
} __attribute__((packed));

#ifdef WIN32
#pragma pack()
#endif


#endif  // CARINABUSDEBUGGER_TIMESTAMPEDBYTESHEADER_H
