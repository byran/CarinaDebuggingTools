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
}

void TimeslotGUIWidgets::UpdateWidgets()
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
