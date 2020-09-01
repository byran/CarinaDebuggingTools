#include "SerialPort.h"
#include <Windows.h>

//HANDLE
SerialPort::SerialPort(char const* portName)
	: hComm{INVALID_HANDLE_VALUE}
{
	hComm = CreateFile(portName,
						GENERIC_READ | GENERIC_WRITE,
						0,
						0,
						OPEN_EXISTING,
						0, // Not overlapped
						0);
	if (hComm == INVALID_HANDLE_VALUE)
	{
		throw UnableToOpenSerialPort();
	}

	try
	{
		ConfigureSerialPort();
	}
	catch(...)
	{
		CloseHandle(hComm);
		throw;
	}
}

void SerialPort::ConfigureSerialPort()
{
	DCB dcb;

	FillMemory(&dcb, sizeof(dcb), 0);
	dcb.DCBlength = sizeof(dcb);
	if (!BuildCommDCB("2000000,n,8,2", &dcb) || !SetCommState(hComm, &dcb))
	{
		throw UnableToOpenSerialPort();
	}

	COMMTIMEOUTS timeouts;
	FillMemory(&timeouts, sizeof(timeouts), 0);
	if(!GetCommTimeouts(hComm, &timeouts))
	{
		throw UnableToOpenSerialPort();
	}

	timeouts.ReadIntervalTimeout = MAXDWORD;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;

	if(!SetCommTimeouts(hComm, &timeouts))
	{
		throw UnableToOpenSerialPort();
	}
}

SerialPort::~SerialPort()
{
	CloseHandle(hComm);
}

void SerialPort::Write(void const* buffer, size_t length)
{
	DWORD bytesWritten{0};
	WriteFile(hComm, buffer, length, &bytesWritten, nullptr);
}

size_t SerialPort::Read(void* buffer, size_t length)
{
	DWORD bytesRead{0};
	return ReadFile(hComm, buffer, length, &bytesRead, nullptr) ? bytesRead : 0;
}
