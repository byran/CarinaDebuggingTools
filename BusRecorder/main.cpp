#include "SerialPort.h"

#include <iostream>
#include <string>
#include <sys/time.h>
#include <chrono>
#include <thread>
#include <signal.h>
#include <fstream>

using namespace std::string_literals;

volatile bool running = true;

struct blockHeader
{
	uint32_t	time;
	uint32_t	length;
} __attribute__((packed));

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
	if(argc < 3)
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

	while(running)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds{10});

		char buffer[16384];
		auto readLength = port.Read(buffer, sizeof(buffer));

		if(readLength)
		{
			blockHeader header;
			header.time = getTimeDelta(startTime);
			header.length = readLength;

			file.write(reinterpret_cast<char const*>(&header), sizeof(header));
			file.write(buffer, readLength);
		}
	}

	std::cout << "exited" << "\n";

	return 0;
}
