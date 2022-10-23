#pragma once
#include "FModManager.h"
#include <vector>

class SoundUI {
protected:
	FModManager* m_fmod_manager;

public:
	typedef enum {
		SAVE,
		LOAD,
		IDLE
	}menu;

    bool is_my_tool_active;
	menu menuState;
	ChannelGroup* m_channel;
	std::map<std::string, Sound*>::iterator itSounds;

	// Constructor
    SoundUI(FModManager* fmod_manager)
			: m_fmod_manager(fmod_manager), is_my_tool_active(true), m_channel(nullptr), menuState(IDLE){}

	// LifeCycle method thats going to be inside the main program loop
	void render();
};

