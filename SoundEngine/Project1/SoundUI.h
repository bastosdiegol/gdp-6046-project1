#pragma once
#include "FModManager.h"

class SoundUI {
protected:
	FModManager* m_fmod_manager;


public:
    bool is_my_tool_active;
	ChannelGroup* m_master;
	ChannelGroup* m_music;
	ChannelGroup* m_fx;

	// Constructor
    SoundUI(FModManager* fmod_manager, ChannelGroup* master, ChannelGroup* music, ChannelGroup* fx)
			: m_fmod_manager(fmod_manager), is_my_tool_active(true), m_master(master), m_music(music), m_fx(fx) {}

	// LifeCycle method thats going to be inside the main program loop
	void render();
};

