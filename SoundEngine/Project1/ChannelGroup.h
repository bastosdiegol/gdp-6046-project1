#pragma once
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>
#include <string>

class ChannelGroup {
public:
	FMOD::ChannelGroup* m_group; // FMOD Channel Group
	float m_pan;				// Pan variable
	float m_pitch;				// Pitch variable

	// Default Constructor
	ChannelGroup();
	// Overloaded Constructor
	// Receives pointer of a Channel Group
	ChannelGroup(FMOD::ChannelGroup* channel_group);
	// Overloaded Constructor
	// Receives pointer of a Channel Group, pan and pitch
	ChannelGroup(FMOD::ChannelGroup* channel_group, float pan, float pitch);
	// Destructor
	~ChannelGroup();
private:

};

