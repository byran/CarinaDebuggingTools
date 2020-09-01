#include "RecordedLogFileWriter.h"

#include "TimestampedBytesHeader.h"

#include <cstdint>

using namespace ::std::chrono;

RecordedLogFileWriter::RecordedLogFileWriter(char const* name)
	: file(name, std::ios_base::binary)
{
	startTime = steady_clock::now();
}

void RecordedLogFileWriter::Write(void const* buffer, size_t length)
{
	auto now = steady_clock::now();
	auto delta = duration_cast<milliseconds>(now - startTime);
	TimestampedBytesHeader header;
	header.time = delta.count();
	header.length = length;

	file.write(reinterpret_cast<char const*>(&header), sizeof(header));
	file.write(static_cast<char const*>(buffer), length);
}
