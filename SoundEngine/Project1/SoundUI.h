#pragma once
#include "FModManager.h"

class SoundUI {
protected:
	FModManager* m_fmod_manager;

public:
	bool is_my_tool_active;

	// Constructor
	SoundUI(FModManager* fmod_manager) : m_fmod_manager(fmod_manager), is_my_tool_active(true) {}
	// LifeCycle method thats going to be inside the main program loop
	void render();
};

