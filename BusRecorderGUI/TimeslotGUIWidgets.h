#ifndef CARINABUSDEBUGGER_TIMESLOTGUIWIDGETS_H
#define CARINABUSDEBUGGER_TIMESLOTGUIWIDGETS_H

#include <sdl_cpp/sdl_cpp.h>
#include <sdl_cpp/widgets/widgets.h>

#include <string>

struct TimeslotGUIWidgets
{
	static unsigned int const labelYSpacing{28};
	static SDL_Color constexpr activeColour{0xFF, 0xFF, 0xFF, 0xFF};
	static SDL_Color constexpr inactiveColour{0x55, 0x55, 0x55, 0xFF};

	sdl::widgets::label index;
	sdl::widgets::label packetsReceived;

	bool active{false};

	void InitialiseLabels(int i);

	void AddModuleTypeToSlot(std::string type);

	void SetLocation(int x, int y);

	unsigned int packetCount{0};
	void IncrementPacketCount() { ++packetCount; }

	void UpdateWidgets();
};



#endif //CARINABUSDEBUGGER_TIMESLOTGUIWIDGETS_H
