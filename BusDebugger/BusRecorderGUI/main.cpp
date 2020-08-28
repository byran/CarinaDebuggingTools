#include <sdl_cpp/sdl_cpp.h>
#include <sdl_cpp/widgets/widgets.h>

#include <fstream>
#include <iostream>

#include "SerialPort.h"
#include "RecordedLogFileWriter.h"
#include "Decoder.h"
#include "TimestampedBytesHeader.h"

#include "FPSMonitor.h"
#include "GUI.h"

using namespace std;

void RunWithSerialPort(char** argv)
{
	GUI gui;

	SerialPort port{argv[1]};
	RecordedLogFileWriter logFile{argv[2]};

	Decoder decoder{&gui};

	sdl::widgets::label times{GUI::windowWidth - 100,
							  GUI::windowHeight - GUI::labelYSpacing, "times"};
	times.colour({0xFF, 0xFF, 0xFF, 0xFF});

	FPSMonitor fps;
	while (!gui.app.quit)
	{
		gui.app.events.poll();

		unsigned char buffer[100];
		auto length = port.Read(buffer, sizeof(buffer));
		logFile.Write(buffer, length);
		decoder.DecodeBytes(buffer, length);

		gui.UpdateWidgets();

		times.text(fps.fpsText);
		gui.app.draw();
		fps.FrameDrawComplete();
	}
}

void RunWithFile(char** argv)
{
	GUI gui;

	std::ifstream file{argv[1], std::ios_base::binary};

	Decoder decoder{&gui};

	sdl::widgets::label times{GUI::windowWidth - 100,
							  GUI::windowHeight - GUI::labelYSpacing, "times"};
	times.colour({0xFF, 0xFF, 0xFF, 0xFF});

	FPSMonitor fps;
	while (!gui.app.quit)
	{
		gui.app.events.poll();

		for (int i = 0; i < 1; ++i)
		{
			if (!file.eof())
			{
				TimestampedBytesHeader header;
				file.read(reinterpret_cast<char*>(&header), sizeof(header));

				char buffer[1024];
				file.read(buffer, header.length);

				decoder.DecodeBytes(header.time, buffer, header.length);
			}
		}

		gui.UpdateWidgets();

		times.text(fps.fpsText);
		gui.app.draw();
		fps.FrameDrawComplete();
	}
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "Invalid arguments, command format is:\n";
		std::cerr << argv[0] << " <device> <log file>\n";
		std::cerr << argv[0] << " <existing long file>\n";
		std::cerr << "\ne.g.\n";
		std::cerr << "\t" << argv[0] << " /dev/ttyUSB0 log.bin\n";
		std::cerr << "\t" << argv[0] << " log.bin" << std::endl;
		return 1;
	}

	if (argc == 2)
	{
		RunWithFile(argv);
	}

	if (argc == 3)
	{
		RunWithSerialPort(argv);
	}

	return 0;
}
