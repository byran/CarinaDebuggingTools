#include "RecordedLogFileWriter.h"
#include "SerialPort.h"

#include <chrono>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>

using namespace std::string_literals;

volatile bool running = true;

void SigIntHandler(int)
{
	running = false;
}

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		std::cerr << "Invalid number of arguments\n";
		return -1;
	}

	std::string devicePath = "/dev/"s + argv[1];

	RecordedLogFileWriter logFile{argv[2]};

	SerialPort port{devicePath.c_str()};

	signal(SIGINT, SigIntHandler);

	while (running)
	{
		char buffer[100];
		auto readLength = port.Read(buffer, sizeof(buffer));

		if (readLength)
		{
			logFile.Write(buffer, readLength);
		}

		if (readLength != sizeof(buffer))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds{1});
		}
	}

	std::cout << "exited"
			  << "\n";

	return 0;
}
