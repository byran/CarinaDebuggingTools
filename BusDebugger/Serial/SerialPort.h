#ifndef CARINABUSDEBUGGER_SERIALPORT_H
#define CARINABUSDEBUGGER_SERIALPORT_H

#include <cstddef>
#include <exception>

class SerialPort
{
private:
	int fd{-1};

public:
	SerialPort(char const* portname);

	void Write(void const* buffer, size_t length);
	size_t Read(void* buffer, size_t length);
};

class UnableToOpenSerialPort : public ::std::exception
{
};

#endif  // CARINABUSDEBUGGER_SERIALPORT_H
