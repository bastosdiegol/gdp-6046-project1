#pragma once
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>
#include <string>

class ChannelGroup {
public:
	FMOD::ChannelGroup* m_group;
	float m_pan;
	float m_pitch;

	// Default Constructor
	ChannelGroup();
	// Overloaded Constructors
	ChannelGroup(FMOD::ChannelGroup* channel_group);
	ChannelGroup(FMOD::ChannelGroup* channel_group, float pan, float pitch);
	// Destructor
	~ChannelGroup();
private:

};

