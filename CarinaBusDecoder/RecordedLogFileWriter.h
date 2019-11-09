#ifndef CARINABUSDEBUGGER_RECORDEDLOGFILEWRITER_H
#define CARINABUSDEBUGGER_RECORDEDLOGFILEWRITER_H

#include <sys/time.h>
#include <cstddef>
#include <fstream>

class RecordedLogFileWriter
{
	timeval startTime;
	std::ofstream file;

public:
	explicit RecordedLogFileWriter(char const* name);

	void Write(void const* buffer, size_t length);
};


#endif //CARINABUSDEBUGGER_RECORDEDLOGFILEWRITER_H
