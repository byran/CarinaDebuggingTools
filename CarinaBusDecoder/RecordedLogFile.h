#ifndef CARINABUSDEBUGGER_RECORDEDLOGFILE_H
#define CARINABUSDEBUGGER_RECORDEDLOGFILE_H


#include <cstdint>

class TimestampedBytesDestination
{
public:
	virtual ~TimestampedBytesDestination() = default;
	virtual void DecodeBytes(uint32_t timeMilliseconds, char *buffer, uint32_t bufferLength) = 0;
};

void DecodeRecordedLogFile(char const *filePath, TimestampedBytesDestination *destination);
#endif //CARINABUSDEBUGGER_RECORDEDLOGFILE_H
