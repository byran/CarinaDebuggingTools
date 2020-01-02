#include "TimeslotGUIWidgets.h"

using namespace std;

void TimeslotGUIWidgets::InitialiseLabels(int i)
{
	index.text("Slot "s + to_string(i));
	packetsReceived.text("0");
}

void TimeslotGUIWidgets::AddModuleTypeToSlot(string type)
{
	index.text(index.text() + " (" + type + ")");
}

void TimeslotGUIWidgets::SetLocation(int x, int y)
{
	index.location.x = x;
	index.location.y = y;
	packetsReceived.location.x = x;
	packetsReceived.location.y = y + labelYSpacing;
	whatICanSee.location.x = x;
	whatICanSee.location.y = y + (2 * labelYSpacing);
	variant.location.x = x;
	variant.location.y = y + (3 * labelYSpacing);

	index.colour({0x00, 0x55, 0xFF, 0xFF});
}

void TimeslotGUIWidgets::UpdateWidgets()
{
	if (active)
	{
		packetsReceived.text_and_colour(to_string(packetCount), activeColour);
		whatICanSee.text_and_colour(whatICanSeeText, activeColour);
		variant.text_and_colour(variantText, activeColour);
	}
	else
	{
		packetsReceived.text_and_colour(to_string(packetCount), inactiveColour);
		whatICanSee.text_and_colour(whatICanSeeText, inactiveColour);
		variant.text_and_colour(variantText, inactiveColour);
	}
}
