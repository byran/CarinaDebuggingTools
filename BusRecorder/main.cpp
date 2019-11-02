#include "SerialPort.h"
#include "TimestampedBytesHeader.h"

#include <signal.h>
#include <sys/time.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

using namespace std::string_literals;

volatile bool running = true;

void sigIntHandler(int)
{
	running = false;
}

uint32_t getTimeDelta(timeval startTime)
{
	timeval now;
	gettimeofday(&now, nullptr);

	timeval delta;

	timersub(&now, &startTime, &delta);

	return (delta.tv_sec * 1000) + (delta.tv_usec / 1000);
}

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		std::cerr << "Invlaid number of arguments\n";
		return -1;
	}

	std::string devicePath = "/dev/"s + argv[1];

	std::ofstream file(argv[2], std::ios_base::binary);

	timeval startTime;
	gettimeofday(&startTime, nullptr);

	SerialPort port{devicePath.c_str()};

	signal(SIGINT, sigIntHandler);

	while (running)
	{
		char buffer[100];
		auto readLength = port.Read(buffer, sizeof(buffer));

		if (readLength)
		{
			TimestampedBytesHeader header;
			header.time = getTimeDelta(startTime);
			header.length = readLength;

			file.write(reinterpret_cast<char const*>(&header), sizeof(header));
			file.write(buffer, readLength);
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
