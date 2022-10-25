#pragma once
#include <vector>
#include "FModManager.h"
#include "Localization.h"

class SoundUI {
protected:
	FModManager* m_fmod_manager;
	Localization* m_localization;

public:
	typedef enum {
		NEWGAME,
		SAVE,
		LOAD,
		LANG,
		IDLE
	}menu;

    bool is_my_tool_active;
	menu menuState;
	ChannelGroup* m_channel;
	std::map<std::string, Sound*>::iterator itSounds;
	std::string newLang;

	// Constructor
    SoundUI(FModManager* fmod_manager, Localization* localization)
			: m_fmod_manager(fmod_manager), m_localization(localization), is_my_tool_active(true), m_channel(nullptr), menuState(SoundUI::IDLE){}

	// LifeCycle method thats going to be inside the main program loop
	void render();
};

