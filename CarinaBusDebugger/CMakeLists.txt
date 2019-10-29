add_executable(CarinaBusDebugger
		main.cpp
		)

target_link_libraries(CarinaBusDebugger PUBLIC
		Carina::Bus
		sdl-cpp
		)
