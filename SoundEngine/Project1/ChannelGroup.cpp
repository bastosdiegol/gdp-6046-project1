#include "ChannelGroup.h"

#define DEBUG_LOG_ENABLED
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

ChannelGroup::ChannelGroup() {
	DEBUG_PRINT("ChannelGroup::ChannelGroup()\n");
	m_group = nullptr;
	m_pan	= 0.0f;
	m_pitch	= 1.0f;
}

ChannelGroup::ChannelGroup(FMOD::ChannelGroup* channel_group) {
	DEBUG_PRINT("ChannelGroup::ChannelGroup(FMOD::ChannelGroup*)\n");
	m_group = channel_group;
	m_pan	= 0.0f;
	m_pitch = 1.0f;
}

ChannelGroup::ChannelGroup(FMOD::ChannelGroup* channel_group, float pan, float pitch) {
	DEBUG_PRINT("ChannelGroup::ChannelGroup(FMOD::ChannelGroup*, pan %f, pitch %f)\n", pan, pitch);
	m_group = channel_group;
	m_pan = pan;
	m_pitch = pitch;
}

ChannelGroup::~ChannelGroup() {
	DEBUG_PRINT("ChannelGroup::~ChannelGroup()\n");
	m_group->release();
}
