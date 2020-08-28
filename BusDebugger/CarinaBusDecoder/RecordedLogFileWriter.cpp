#include "RecordedLogFileWriter.h"

#include "TimestampedBytesHeader.h"

#include <cstdint>

RecordedLogFileWriter::RecordedLogFileWriter(char const* name)
	: file(name, std::ios_base::binary)
{
	startTime = ::std::chrono::steady_clock::now();
}

using namespace ::std::chrono;

void RecordedLogFileWriter::Write(void const* buffer, size_t length)
{
	auto now = ::std::chrono::steady_clock::now();
	auto delta = duration_cast<std::chrono::milliseconds>(now - startTime);
	TimestampedBytesHeader header;
	header.time = delta.count();
	header.length = length;

	file.write(reinterpret_cast<char const*>(&header), sizeof(header));
	file.write(static_cast<char const*>(buffer), length);
}
