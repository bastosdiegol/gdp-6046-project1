#pragma once
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>
#include <map>
#include <string>

#include "ChannelGroup.h"
#include "Sound.h"

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
	// Gets the pointer of the Mute Status of a specified channgel
	// Receives channel name and pointer to status, Returns Nothing
	// True = silent. False = audible
	void getChannelGroupMuteStatus(const std::string& name, bool* muteStatus);
	// Sets a new bool value to the Mute Status of a specified channel
	// Receives channel name and new status, Returns Nothing
	// True = silent. False = audible
	void setChannelGroupMuteStatus(const std::string& name, bool muteStatus);
	// Gets pointer to the float value of the pan
	// Receives channel name and pointer to pan, Returns Nothing
	void getChannelGroupPan(const std::string& name, float* pan);
	// Sets the pan of a especified channel
	// Receives channel name and desired pan, Returns Nothing
	void setChannelGroupPan(const std::string& name, float pan);

	// Create a DSP
	// Accepts FMOD_DSP_TYPE. Returns Nothing.
	void createDSP(FMOD_DSP_TYPE dsp_type);
	// Load All DSPs intented to use on the engine
	void loadDSPs();
	// Set a Float Value to a Specified parameter of a FMOD_DSP_TYPE
	// Instead of Letting other Classes Call DSP.setParameter directly we gonna do through this and check for errors
	// Accepts FMOD_DSP_TYPE, Int Parameter Index and Float Value. Returns Nothing.
	void setFloatParameterDSP(FMOD_DSP_TYPE dsp_type, int fmdDspParameter, float value);
	// Set a Bool Value to a Specified parameter of a FMOD_DSP_TYPE
	// Instead of Letting other Classes Call DSP.setParameter directly we gonna do through this and check for errors
	// Accepts FMOD_DSP_TYPE, Int Parameter Index and Bool Value. Returns Nothing.
	void setBoolParameterDSP(FMOD_DSP_TYPE dsp_type, int fmdDspParameter, bool value);
	// Set a Int Value to a Specified parameter of a FMOD_DSP_TYPE
	// Instead of Letting other Classes Call DSP.setParameter directly we gonna do through this and check for errors
	// Accepts FMOD_DSP_TYPE, Int Parameter Index and Int Value. Returns Nothing.
	void setIntParameterDSP(FMOD_DSP_TYPE dsp_type, int fmdDspParameter, int value);
	// Gets a Float Pointer to a Specified parameter of a FMOD_DSP_TYPE
	// Instead of Letting other Classes Call DSP.setParameter directly we gonna do through this and check for errors
	// Accepts FMOD_DSP_TYPE, Int Parameter Index and Float Pointer. Returns Nothing.
	void getFloatParameterDSP(FMOD_DSP_TYPE dsp_type, int fmdDspParameter, float* value);
	// Adds a DSP Effect on a specified channel group
	// Accepts Channel name and FMOD DSP Type. Returns nothing.
	void addDSPEffect(const std::string& name, const FMOD_DSP_TYPE& dsp_type);
	// Removes a DSP Effect from a specified channel group
	// Accepts Channel name and FMOD DSP Type. Returns nothing.
	void removeDSPEffect(const std::string& name, const FMOD_DSP_TYPE& dsp_type);

	// Loads all sounds available from XML File
	void loadSoundsFromFile();
	// Play a sound on a specified channel group
	void playSound(const std::string& sound_name, const std::string& channel_group_name);
	// Stop all the sounds on a specified channel group
	void stopSound(const std::string& channel_group_name);
	// Sets bool pause on a specified channel group
	void setPause(const std::string& channel_group_name, const bool pause);

private:
	const int			MAX_CHANNELS = 255;
	const std::string	SOUND_FILE	 = "sounds.xml";

	FMOD_RESULT							 m_result;			// Variable to check Erros
	FMOD::System*						 m_system;			// FMod System
	std::map<std::string, ChannelGroup*> m_channel_groups;	// Map Tree of Channel Groups
	std::map<std::string, Sound*>		 m_sounds;			// Map Tree of Sounds
	std::map<FMOD_DSP_TYPE, FMOD::DSP*>	 m_dsp;				// Map Tree of Digital Sound Processors

	friend class SoundUI; // We will allow SoundUI to access these private variables above
};

