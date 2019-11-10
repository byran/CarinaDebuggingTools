#include <sdl_cpp/fonts/ttf_font.h>
#include <sdl_cpp/fonts/ttf_font_context.h>
#include <sdl_cpp/sdl_cpp.h>
#include <sdl_cpp/widgets/widgets.h>

#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cstdint>
#include <bitset>

#include "TimestampedBytesHeader.h"
#include "RawPacketDestination.h"
#include "Decoder.h"

#include "Bus/Internal/Packets.h"

#include "SourceCodeProFont.h"

using namespace std;

struct TimeslotGUIWidgets
{
	static unsigned int const labelYSpacing{28};
	static SDL_Color constexpr activeColour{ 0xFF, 0xFF, 0xFF, 0xFF };
	static SDL_Color constexpr inactiveColour{ 0x55, 0x55, 0x55, 0xFF };

	sdl::widgets::label index;
	sdl::widgets::label packetsReceived;

	bool active{false};

	void InitialiseLabels(int i)
	{
		index.text("Slot "s + to_string(i));
		packetsReceived.text("0");
	}

	void AddModuleTypeToSlot(string type)
	{
		index.text(index.text() + " (" + type + ")");
	}

	void SetLocation(int x, int y)
	{
		index.location.x = x;
		index.location.y = y;
		packetsReceived.location.x = x;
		packetsReceived.location.y = y + labelYSpacing;
	}

	unsigned int packetCount{0};
	void IncrementPacketCount()
	{
		++packetCount;
	}

	void UpdateWidgets()
	{
		packetsReceived.text(to_string(packetCount));
		if (active)
		{
			index.colour(activeColour);
			packetsReceived.colour(activeColour);
		}
		else
		{
			index.colour(inactiveColour);
			packetsReceived.colour(inactiveColour);
		}
	}
};

struct GUI : public RawPacketDestination
{
	static unsigned int const labelYSpacing{28};

	static unsigned int const numberOfTimeslotRows{4};
	static unsigned int const timeslotHeight{(TimeslotGUIWidgets::labelYSpacing * 2) + 6};

	static unsigned int const mapLabelTop{numberOfTimeslotRows * timeslotHeight};

	static unsigned int const andLabelTop{mapLabelTop + labelYSpacing + 6};

	static unsigned int const orLabelTop{andLabelTop + labelYSpacing + 6};

	static unsigned int const logLabelTop{orLabelTop + labelYSpacing + 6};
	static unsigned int const numberOfLogLabels{20};

	static unsigned int const windowWidth{1200};
	static unsigned int const windowHeight{logLabelTop + (numberOfLogLabels * labelYSpacing)};

	static SDL_Color constexpr invalidColour = { 0xDD, 0x00, 0x00, 0xFF };
	static SDL_Color constexpr validColour = { 0xFF, 0xFF, 0xFF, 0xFF };

	sdl::widgets::window_application app;
	sdl::widgets::label mapLabel;
	string mapText;
	sdl::widgets::label andLabel;
	string andText;
	SDL_Point mapLinesPoints[2] = { { 0, mapLabelTop - 2 }, { windowWidth, mapLabelTop - 2 } };
	sdl::widgets::lines_no_storage mapLines{mapLinesPoints};
	sdl::widgets::label orLabel;
	string orText;
	SDL_Point logLinesPoints[2] = { { 0, logLabelTop - 2 }, { windowWidth, logLabelTop - 2 } };
	sdl::widgets::lines_no_storage logLines{logLinesPoints};
	sdl::widgets::label logLabels[numberOfLogLabels];
	string logText[numberOfLogLabels];
	SDL_Color logTextColour[numberOfLogLabels]{};

	TimeslotGUIWidgets timeslot[NUMBER_OF_BUS_TIMESLOTS];

	GUI(int argc, char** argv)
		: app{argc, argv, "Bus Recorder", 0, 0,
			  windowWidth, windowHeight, false}
	{
		app.font = sdl::fonts::ttf_font{SourceCodePro_Medium_ttf, SourceCodePro_Medium_ttf_len, 24};
		app.colour({ 0x00, 0x00, 0x00, 0xFF });
		SetLabelPositions();

		int x = 0;
		int y = 0;

		for(int i = 0; i < NUMBER_OF_BUS_TIMESLOTS; ++i)
		{
			if((i != 0) && (i % (NUMBER_OF_BUS_TIMESLOTS / numberOfTimeslotRows) == 0))
			{
				x = 0;
				y += timeslotHeight;
			}

			timeslot[i].SetLocation(x, y);
			timeslot[i].InitialiseLabels(i);

			x += (windowWidth / (NUMBER_OF_BUS_TIMESLOTS / numberOfTimeslotRows));
		}

		logLines.colour(validColour);
		mapLines.colour(validColour);

		mapLabel.colour(validColour);
		mapLabel.location.y = mapLabelTop;
		andLabel.colour(validColour);
		andLabel.location.y = andLabelTop;
		orLabel.colour(validColour);
		orLabel.location.y = orLabelTop;

		timeslot[0].AddModuleTypeToSlot("R");
		timeslot[1].AddModuleTypeToSlot("A");
		timeslot[2].AddModuleTypeToSlot("A");
		timeslot[3].AddModuleTypeToSlot("A");
		timeslot[4].AddModuleTypeToSlot("A");
		timeslot[5].AddModuleTypeToSlot("B");
		timeslot[6].AddModuleTypeToSlot("D");
		timeslot[7].AddModuleTypeToSlot("D");
		timeslot[8].AddModuleTypeToSlot("D");
		timeslot[9].AddModuleTypeToSlot("D");
		timeslot[19].AddModuleTypeToSlot("Q");
	}

	void SetLabelPositions()
	{
		int y = logLabelTop;
		for_each(begin(logLabels), end(logLabels),
				 [&y](sdl::widgets::label& p)
				 {
					 p.location.y = y;
					 y += labelYSpacing;
				 });
	}

	void UpdateWidgets()
	{
		auto label = begin(logLabels);
		auto text = begin(logText);
		auto colour = begin(logTextColour);
		for(; label != end(logLabels); ++label, ++text, ++colour)
		{
			label->text_and_colour(*text, *colour);
		}

		mapLabel.text(mapText);
		andLabel.text(andText);
		orLabel.text(orText);

		for(auto& t : timeslot)
		{
			t.UpdateWidgets();
		}
	}

	void AddLogEntry(string text, SDL_Color colour = validColour)
	{
		logText[0] = move(text);
		logTextColour[0] = colour;
		rotate(begin(logText), begin(logText) + 1, end(logText));
		rotate(begin(logTextColour), begin(logTextColour) + 1, end(logTextColour));
	}

	void NonPacketBytesReceived(unsigned char* bytes, unsigned int length, uint32_t time) override
	{
		AddLogEntry("Bytes");
	}

	unsigned int packetCount{0};
	void PacketReceived(BusHeader* header, unsigned int totalPacketLength, uint32_t time) override
	{
		stringstream s;
		SDL_Color colour = validColour;

		++packetCount;
		s << setw(8) << setfill('0') << packetCount << " : ";

		switch(header->packetType)
		{
		case BPT_NORMAL_SYNC:
			s << "Sync            ";
			ProcessNormalSyncPacket(reinterpret_cast<BusSyncPacket*>(header));
			colour = SDL_Color{ 0x00, 0xAA, 0x00, 0xFF };
			break;
		case BPT_VALUES:
			s << "Values          ";
			ProcessValuesPacket(reinterpret_cast<BusValuesPacketPreamble*>(header));
			break;
		case BPT_TIMESLOT_IN_USE:
			s << "Time slot in use";
			break;
		default:
			s << "Unknown packet type : " << static_cast<int>(header->packetType);
			colour = invalidColour;
			break;
		}

		AddLogEntry(s.str(), colour);
	}

	void PacketWithInvalidCrcReceived(BusHeader* header, uint32_t time) override
	{
		AddLogEntry("Invalid packet");
	}

	void ProcessNormalSyncPacket(BusSyncPacket* packet)
	{
		for(unsigned int i = 0; i < NUMBER_OF_BUS_TIMESLOTS; ++i)
		{
			timeslot[i].active = (packet->data.map & (1u << i));
		}

		{
			uint32_t mask = (1 << NUMBER_OF_BUS_TIMESLOTS) - 1;
			stringstream s;
			bitset<NUMBER_OF_BUS_TIMESLOTS> mapBits{packet->data.map & mask};
			s << "Map : " << mapBits;
			mapText = s.str();
		}

		{
			stringstream s;
			bitset<32> andBits{packet->data.andValues};
			s << "And : " << andBits;
			andText = s.str();
		}

		{
			stringstream s;
			bitset<32> orBits{packet->data.orValues};
			s << "Or  : " << orBits;
			orText = s.str();
		}
	}

	void ProcessValuesPacket(BusValuesPacketPreamble* packet)
	{
		auto const& slot = packet->data.slot;
		for(unsigned int i = 0; i < NUMBER_OF_BUS_TIMESLOTS; ++i)
		{
			if(slot & (1u << i))
			{
				timeslot[i].IncrementPacketCount();
				break;
			}
		}
	}
};

#include <chrono>

int main(int argc, char** argv)
{
	if(argc < 2)
		return 1;

	GUI gui{argc, argv};

	std::ifstream file{argv[1], std::ios_base::binary};

	Decoder decoder{&gui};

	sdl::widgets::label times{ GUI::windowWidth - 100, GUI::windowHeight - GUI::labelYSpacing, "times"};
	times.colour({ 0xFF, 0xFF, 0xFF, 0xFF });

	using timer = std::chrono::system_clock;
	string timesText;
	unsigned int timesCount{0};
	auto startTime = timer::now();
	while (!gui.app.quit)
	{
		if(timesCount == 0)
		{
			startTime = timer::now();
		}
		gui.app.events.poll();

		for(int i = 0; i < 1; ++i)
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

		times.text(timesText);
		gui.app.draw();
		if(timesCount == 25)
		{
			auto endTime = timer::now();
			timesCount = 0;

			auto total = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
			timesText = to_string(1000000 * 25 / (total.count() ? total.count() : 1)) + "fps";
		}
		else
		{
			++timesCount;
		}
	}

	return 0;
}
