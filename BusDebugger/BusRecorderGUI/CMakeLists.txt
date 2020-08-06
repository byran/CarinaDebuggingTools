add_executable(BusRecorderGUI
		FPSMonitor.cpp
		FPSMonitor.h
		GUI.cpp
		GUI.h
		main.cpp
		TimeslotGUIWidgets.cpp
		TimeslotGUIWidgets.h
		)

if(${UNIX})
	target_compile_definitions(BusRecorderGUI PUBLIC SERIAL_SUPPORT)

	set(BusRecorderGUISerialLibraries
			Serial
			)
endif(${UNIX})

target_link_libraries(BusRecorderGUI PUBLIC
		CarinaBusDecoder
		sdl-cpp
		SourceCodeProFont
		${BusRecorderGUISerialLibraries}
		)

copy_sdl2_windows_dlls("${CarinaBusDebugger_BINARY_DIR}/BusRecorderGUI")
