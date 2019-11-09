#include "RecordedLogFileWriter.h"

#include "TimestampedBytesHeader.h"

#include <cstdint>

namespace
{
	inline uint32_t GetTimeDelta(timeval startTime)
	{
		timeval now;
		gettimeofday(&now, nullptr);

		timeval delta;
		timersub(&now, &startTime, &delta);

		return (delta.tv_sec * 1000) + (delta.tv_usec / 1000);
	}
}

RecordedLogFileWriter::RecordedLogFileWriter(char const* name)
	: file(name, std::ios_base::binary)
{
	gettimeofday(&startTime, nullptr);
}

void RecordedLogFileWriter::Write(void const* buffer, size_t length)
{
	TimestampedBytesHeader header;
	header.time = GetTimeDelta(startTime);
	header.length = length;

	file.write(reinterpret_cast<char const*>(&header), sizeof(header));
	file.write(static_cast<char const*>(buffer), length);
}
