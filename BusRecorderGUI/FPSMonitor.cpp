#include "FPSMonitor.h"

using namespace std;

void FPSMonitor::FrameDrawComplete()
{
	if(timesCount == fpsCalculateRate)
	{
		auto endTime = timer::now();

		auto total = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
		auto fps = 1000000 * fpsCalculateRate / (total.count() ? total.count() : 1);
		fpsCalculateRate = fps / 2;
		fpsText = to_string(fps) + "fps";

		timesCount = 0;
		startTime = endTime;
	}
	else
	{
		++timesCount;
	}
}
