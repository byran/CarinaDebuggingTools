#include "SerialPort.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

namespace
{
	/*
	 The values for parity are 0 (meaning no parity),
	 PARENB|PARODD (enable parity and use odd),
	 PARENB (enable parity and use even),
	 PARENB|PARODD|CMSPAR (mark parity),
	 and PARENB|CMSPAR (space parity)
	*/
	int set_interface_attribs(int fd, int speed, int parity)
	{
		struct termios tty;
		memset(&tty, 0, sizeof tty);
		if (tcgetattr(fd, &tty) != 0)
		{
			// error_message ("error %d from tcgetattr", errno);
			return -1;
		}

		cfsetospeed(&tty, speed);
		cfsetispeed(&tty, speed);

		tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;  // 8-bit chars
		// disable IGNBRK for mismatched speed tests; otherwise receive break
		// as \000 chars
		tty.c_iflag &= ~IGNBRK;  // disable break processing
		tty.c_lflag = 0;		 // no signaling chars, no echo,
		// no canonical processing
		tty.c_oflag = 0;	  // no remapping, no delays
		tty.c_cc[VMIN] = 0;   // read doesn't block
		tty.c_cc[VTIME] = 0;  // 0.5 seconds read timeout

		tty.c_iflag &= ~(IXON | IXOFF | IXANY);  // shut off xon/xoff ctrl

		tty.c_cflag |= (CLOCAL | CREAD);  // ignore modem controls,
		// enable reading
		tty.c_cflag &= ~(PARENB | PARODD);  // shut off parity
		tty.c_cflag |= parity;
		tty.c_cflag &= ~CSTOPB;
		tty.c_cflag &= ~CRTSCTS;

		cfmakeraw(&tty);

		if (tcsetattr(fd, TCSANOW, &tty) != 0)
		{
			// error_message ("error %d from tcsetattr", errno);
			return -1;
		}
		return 0;
	}

	void set_blocking(int fd, int should_block)
	{
		struct termios tty;
		memset(&tty, 0, sizeof tty);
		if (tcgetattr(fd, &tty) != 0)
		{
			// error_message ("error %d from tggetattr", errno);
			return;
		}

		tty.c_cc[VMIN] = should_block ? 1 : 0;
		tty.c_cc[VTIME] = 5;  // 0.5 seconds read timeout

		if (tcsetattr(fd, TCSANOW, &tty) != 0)
		{
			// error_message("error %d setting term attributes", errno);
		}
	}
}  // namespace
// char *portname = "/dev/ttyUSB1"
SerialPort::SerialPort(char const* portname)
{
	fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0)
	{
		// error_message ("error %d opening %s: %s", errno, portname, strerror
		// (errno));
		return;
	}

	set_interface_attribs(fd, B2000000,
						  0);  // set speed to 2,000,000 bps, 8n1 (no parity)
	// set_blocking (fd, 0);                // set no blocking
}

void SerialPort::Write(void const* buffer, size_t length)
{
	write(fd, buffer, length);
}

size_t SerialPort::Read(void* buffer, size_t length)
{
	return read(fd, buffer, length);
}
