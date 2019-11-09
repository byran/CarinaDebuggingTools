#include <iostream>

#include <sdl_cpp/fonts/ttf_font.h>
#include <sdl_cpp/fonts/ttf_font_context.h>
#include <sdl_cpp/sdl_cpp.h>
#include <sdl_cpp/widgets/widgets.h>

#include <chrono>
#include <thread>

int main(int argc, char** argv)
{
	sdl::widgets::window_application app{argc, argv, "Bus Debugger", 0, 0,
										 1000, 900};
	sdl::mouse_state mouse{app.mouse_events};

	app.resize_font(16);

	sdl::widgets::label value[748];
	int const rows = 44;

	int i = 0;
	for (auto&& v : value)
	{
		int const y = ((i % rows) * 20) + 10;
		int const x = ((i / rows) * 56) + 10;
		v.location.x = x;
		v.location.y = y;

		v.text(std::to_string(i));
		++i;
	}

	i = 0;
	std::chrono::milliseconds sleepTime{16};
	while (!app.quit)
	{
		app.events.poll();
		++i;
		i %= 1000;

		for (auto&& v : value)
		{
			v.text(std::to_string(i));
		}

		app.draw();
		std::this_thread::sleep_for(sleepTime);
	}
	return 0;
}
