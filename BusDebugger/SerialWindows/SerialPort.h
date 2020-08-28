#ifndef CARINABUSDEBUGGER_SERIALPORT_H
#define CARINABUSDEBUGGER_SERIALPORT_H

#include <exception>

class SerialPort
{
private:
	void* hComm;

	void ConfigureSerialPort();
public:
	explicit SerialPort(char const* portName);
	~SerialPort();

	void Write(void const* buffer, size_t length);
	size_t Read(void* buffer, size_t length);
};

class UnableToOpenSerialPort : public ::std::exception
{
};

#endif	// CARINABUSDEBUGGER_SERIALPORT_H
