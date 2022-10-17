#pragma once
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>
#include <map>
#include <string>

#include "ChannelGroup.h"
#include "Sound.h"


enum DSP_ID {
	CHORUS = 0, COMPRESSOR = 1, DELAY = 2, DISTORTION = 3, ECHO = 4, FADER = 5,
	FLANGE = 6, NORMALIZE = 7, OSCILATOR = 8, SFXREVERB = 9, TREMOLO = 10
};


class FModManager {
public:
	std::map<std::string, Sound*>		m_sounds; // Map Tree of Sounds
	std::map<DSP_ID, FMOD::DSP*>		m_dsp;	  // Map Tree of Digital Sound Processors

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
	// Gets pointer to the float value of the pan
	// Receives channel name and pointer to pan, Returns Nothing
	void getChannelGroupPan(const std::string& name, float* pan);
	// Sets the pan of a especified channel
	// Receives channel name and desired pan, Returns Nothing
	void setChannelGroupPan(const std::string& name, float pan);

	// Create a DSP
	void createDSP(const DSP_ID& id, FMOD_DSP_TYPE dsp_type);
	// Load All DSPs intented to use on the engine
	void loadDSPs();

	// Loads all sounds available from XML File
	void loadSoundsFromFile();
	// Play a sound on a specified channel group
	void playSound(const std::string& sound_name, const std::string& channel_group_name);
	// Stop all the sounds on a specified channel group
	void stopSound(const std::string& channel_group_name);
	// Sets bool pause on a specified channel group
	void setPause(const std::string& channel_group_name, const bool pause);

protected:
	FMOD_RESULT		m_result; // Variable to check Erros
	FMOD::System*	m_system; // FMod System
	std::map<std::string, ChannelGroup*> m_channel_groups; // Map Tree of Channel Groups

private:
	const int			MAX_CHANNELS = 255;
	const std::string	SOUND_FILE	 = "sounds.xml";
};

