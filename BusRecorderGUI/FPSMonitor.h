#ifndef CARINABUSDEBUGGER_FPSMONITOR_H
#define CARINABUSDEBUGGER_FPSMONITOR_H

#include <chrono>
#include <string>

class FPSMonitor
{
private:
	using timer = std::chrono::system_clock;
	unsigned int timesCount{0};
	unsigned int fpsCalculateRate{10};
	decltype(timer::now()) startTime{timer::now()};

public:
	std::string fpsText;

	void FrameDrawComplete();
};


#endif //CARINABUSDEBUGGER_FPSMONITOR_H
