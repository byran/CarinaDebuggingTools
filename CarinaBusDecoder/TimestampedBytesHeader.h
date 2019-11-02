#ifndef CARINABUSDEBUGGER_TIMESTAMPEDBYTESHEADER_H
#define CARINABUSDEBUGGER_TIMESTAMPEDBYTESHEADER_H

#include <cstdint>

struct TimestampedBytesHeader
{
	uint32_t time;
	uint32_t length;
} __attribute__((packed));

#endif  // CARINABUSDEBUGGER_TIMESTAMPEDBYTESHEADER_H
