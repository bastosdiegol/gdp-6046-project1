#include <iostream>

#include "FModManager.h"

#define DEBUG_LOG_ENABLED
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

FModManager::FModManager(const int system_flags) {
	DEBUG_PRINT("FModManager::FModManager(const int system_flags)\n");
	// Creates the System
	m_result = System_Create(&m_system);
	// Checks for errors
	if (m_result != FMOD_OK) {
		std::cout << "fmod error: #" << m_result << "-" << FMOD_ErrorString(m_result) << std::endl;
		return;
	}

	// Initialize the System
	m_result = m_system->init(MAX_CHANNELS, system_flags, nullptr);
	// Checks for errors
	if (m_result != FMOD_OK) {
		std::cout << "fmod error: #" << m_result << "-" << FMOD_ErrorString(m_result) << std::endl;
		return;
	}
}

FModManager::~FModManager() {
	DEBUG_PRINT("FModManager::~FModManager()\n");
	std::map<std::string, ChannelGroup*>::iterator it;
	// Iterates trought map tree of channels and call each destructor
	for (it = m_channel_groups.begin(); it != m_channel_groups.end(); it++)
		it->second->~ChannelGroup();
	// Clear the map tree
	m_channel_groups.clear();
	// Deletes the FMOD System
	m_system->release();
}

void FModManager::createChannelGroup(const std::string& name) {
	DEBUG_PRINT("FModManager::createChannelGroup(const std::string& %s)\n", name.c_str());
	FMOD::ChannelGroup* fmod_channel_group;
	// Creates the FMOD Channel Group
	m_result = m_system->createChannelGroup(name.c_str(), &fmod_channel_group);
	if (m_result != FMOD_OK) {
		std::cout << "fmod error: #" << m_result << "-" << FMOD_ErrorString(m_result) << std::endl;
		return;
	}
	// Creates the Class Channel Group and Inserts the FMOD in
	ChannelGroup* channel_group = new ChannelGroup(fmod_channel_group);
	// Adds the class instane into the member map tree
	m_channel_groups.try_emplace(name, channel_group);
}

void FModManager::removeChannelGroup(const std::string& name) {
	DEBUG_PRINT("FModManager::removeChannelGroup(const std::string& %s)\n", name.c_str());
	// Finds the Channel group by Name
	std::map<std::string, ChannelGroup*>::iterator it = m_channel_groups.find(name);
	// Check if found
	if (it == m_channel_groups.end()) {
		std::cout << "FModManager error: Couldn't find the ChannelGroup named #" << name << std::endl;
		return;
	}
	// Release the FMOD Channel Group
	it->second->~ChannelGroup();
	// Removes the class instance from the member map tree
	m_channel_groups.erase(it);
}
