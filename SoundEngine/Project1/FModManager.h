#pragma once
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>
#include <map>
#include <string>

#include "ChannelGroup.h"

class FModManager {
public:
	// Constructor
	FModManager(const int system_flags);
	// Destructor
	~FModManager();

	// Creates a Channel Group and insert into the manager
	// Receives a name, Returns Nothing
	void createChannelGroup(const std::string& name);
	// Removes a Channel Group and insert into the manager
	// Receives a name, Returns Nothing
	void removeChannelGroup(const std::string& name);
	// Sets parent relation between two channels
	// Receives child and parent name, Returns Nothing
	void setChannelGroupParent(const std::string& parent_name, const std::string& child_name);
	// Gets pointer to the float value of the volume
	// Receives channel name and pointer to volume, Returns Nothing
	void getChannelGroupVolume(const std::string& name, float* volume);
	// Sets the volume of a especified channel
	// Receives channel name and desired volume, Returns Nothing
	void setChannelGroupVolume(const std::string& name, float volume);

protected:
	FMOD_RESULT		m_result; // Variable to check Erros
	FMOD::System*	m_system; // FMod System
	std::map<std::string, ChannelGroup*> m_channel_groups; // Map Tree of Channel Groups

private:
	const int MAX_CHANNELS = 255;
	
};

