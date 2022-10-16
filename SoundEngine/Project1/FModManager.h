#pragma once
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>
#include <map>
#include <string>

#include "ChannelGroup.h"

class FModManager {
public:
	const int MAX_CHANNELS = 255;

	// Constructor
	FModManager(const int system_flags);
	// Destructor
	~FModManager();

	void createChannelGroup(const std::string& name);
	void removeChannelGroup(const std::string& name);

protected:
	FMOD_RESULT		m_result; // Variable to check Erros
	FMOD::System*	m_system; // FMod System
	std::map<std::string, ChannelGroup*> m_channel_groups; // Tree of Channel Groups

private:
	
};

