#ifndef CARINABUSDEBUGGER_GUI_H
#define CARINABUSDEBUGGER_GUI_H

#include <sdl_cpp/sdl_cpp.h>
#include <sdl_cpp/widgets/widgets.h>

#include "TimeslotGUIWidgets.h"

#include "RawPacketDestination.h"

#include "Bus/Internal/Packets.h"

struct GUI : public RawPacketDestination
{
	static unsigned int const labelYSpacing{24};

	static unsigned int const numberOfTimeslotRows{4};
	static unsigned int const timeslotHeight{
		(TimeslotGUIWidgets::labelYSpacing *
		 TimeslotGUIWidgets::numberOfLabels) +
		6};

	static unsigned int const mapLabelTop{numberOfTimeslotRows *
										  timeslotHeight};

	static unsigned int const andLabelTop{mapLabelTop + labelYSpacing + 6};

	static unsigned int const orLabelTop{andLabelTop + labelYSpacing + 6};

	static unsigned int const logLabelTop{orLabelTop + labelYSpacing + 6};
	static unsigned int const numberOfLogLabels{16};

	static unsigned int const windowWidth{1200};
	static unsigned int const windowHeight{logLabelTop +
										   (numberOfLogLabels * labelYSpacing)};

	static SDL_Color constexpr invalidColour = {0xDD, 0x00, 0x00, 0xFF};
	static SDL_Color constexpr validColour = {0xFF, 0xFF, 0xFF, 0xFF};

	sdl::widgets::window_application app;
	sdl::widgets::label mapLabel;
	std::string mapText;
	sdl::widgets::label andLabel;
	std::string andText;
	SDL_Point mapLinesPoints[2] = {{0, mapLabelTop - 2},
								   {windowWidth, mapLabelTop - 2}};
	sdl::widgets::lines_no_storage mapLines{mapLinesPoints};
	sdl::widgets::label orLabel;
	std::string orText;
	SDL_Point logLinesPoints[2] = {{0, logLabelTop - 2},
								   {windowWidth, logLabelTop - 2}};
	sdl::widgets::lines_no_storage logLines{logLinesPoints};
	sdl::widgets::label logLabels[numberOfLogLabels];
	std::string logText[numberOfLogLabels];
	SDL_Color logTextColour[numberOfLogLabels]{};

	TimeslotGUIWidgets timeslot[NUMBER_OF_BUS_TIMESLOTS];

	GUI(int argc, char** argv);

	void SetLabelPositions();

	void UpdateWidgets();
	void AddLogEntry(std::string text, SDL_Color colour = validColour);
	void NonPacketBytesReceived(unsigned char* bytes, unsigned int length,
								uint32_t time) override;

	unsigned int packetCount{0};
	void PacketReceived(BusHeader* header, unsigned int totalPacketLength,
						uint32_t time) override;
	void PacketWithInvalidCrcReceived(BusHeader* header,
									  uint32_t time) override;

	void ProcessNormalSyncPacket(BusSyncPacket* packet);
	void ProcessValuesPacket(BusValuesPacketPreamble* packet);
};

#endif  // CARINABUSDEBUGGER_GUI_H
