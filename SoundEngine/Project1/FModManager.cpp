#include <iostream>
#include <pugixml/pugixml.hpp>

#include "FModManager.h"

#define DEBUG_LOG_ENABLED
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

FModManager::FModManager(const int system_flags) {
	DEBUG_PRINT("FModManager::FModManager(%d)\n", system_flags);
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
	std::map<std::string, Sound*>::iterator itSounds;
	// Iterates trought map tree of channels and call each destructor
	for (itSounds = m_sounds.begin(); itSounds != m_sounds.end(); itSounds++)
		itSounds->second->~Sound();
	std::map<std::string, ChannelGroup*>::iterator itChannels;
	// Iterates trought map tree of channels and call each destructor
	for (itChannels = m_channel_groups.begin(); itChannels != m_channel_groups.end(); itChannels++)
		itChannels->second->~ChannelGroup();
	// Clear the map tree
	m_channel_groups.clear();
	// Deletes the FMOD System
	m_system->release();
}

void FModManager::createChannelGroup(const std::string& name) {
	DEBUG_PRINT("FModManager::createChannelGroup(%s)\n", name.c_str());
	FMOD::ChannelGroup* fmod_channel_group;
	// Creates the FMOD Channel Group
	m_result = m_system->createChannelGroup(name.c_str(), &fmod_channel_group);
	// Checks the result
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
	DEBUG_PRINT("FModManager::removeChannelGroup(%s)\n", name.c_str());
	// Finds the Channel group by Name
	std::map<std::string, ChannelGroup*>::iterator it = m_channel_groups.find(name);
	// Checks if it was found
	if (it == m_channel_groups.end()) {
		std::cout << "FModManager error: Couldn't find the ChannelGroup named #" << name << std::endl;
		return;
	}
	// Release the FMOD Channel Group
	it->second->~ChannelGroup();
	// Removes the class instance from the member map tree
	m_channel_groups.erase(it);
}

void FModManager::setChannelGroupParent(const std::string& parent_name, const std::string& child_name) {
	DEBUG_PRINT("FModManager::setChannelGroupParent(%s, %s)\n", parent_name.c_str(), child_name.c_str());
	// Tries to find both Parent and Child channel groups
	std::map<std::string, ChannelGroup*>::iterator itParent = m_channel_groups.find(parent_name);
	std::map<std::string, ChannelGroup*>::iterator itChild  = m_channel_groups.find(child_name);
	// Checks if both parent and child was found
	if (itParent == m_channel_groups.end() || itChild == m_channel_groups.end()) {
		std::cout << "FModManager error: Couldn't find the ChannelGroup named #" << parent_name 
													<< "or ChannelGroup named #" << child_name << std::endl;
		return;
	}
	// If found tries to estabelish the relation using FMOD
	m_result = itParent->second->m_group->addGroup(itChild->second->m_group);
	// Checks for error
	if (m_result != FMOD_OK) 
		std::cout << "fmod error: #" << m_result << "-" << FMOD_ErrorString(m_result) << std::endl;
}

void FModManager::getChannelGroupVolume(const std::string& name, float* volume) {
	DEBUG_PRINT("FModManager::getChannelGroupVolume(%s)\n", name.c_str());
	// Finds the Channel group by Name
	std::map<std::string, ChannelGroup*>::iterator it = m_channel_groups.find(name);
	// Checks if it was found
	if (it == m_channel_groups.end()) {
		std::cout << "FModManager error: Couldn't find the ChannelGroup named #" << name << std::endl;
		return;
	}
	// Gets the pointer to the value of the volume
	it->second->m_group->getVolume(volume);
}

void FModManager::setChannelGroupVolume(const std::string& name, float volume) {
	DEBUG_PRINT("FModManager::setChannelGroupVolume(%s, %f)\n", name.c_str(), volume);
	// Finds the Channel group by Name
	std::map<std::string, ChannelGroup*>::iterator it = m_channel_groups.find(name);
	// Checks if it was found
	if (it == m_channel_groups.end()) {
		std::cout << "FModManager error: Couldn't find the ChannelGroup named #" << name << std::endl;
		return;
	}
	// Sets the new value for the volume
	it->second->m_group->setVolume(volume);
}

void FModManager::loadSoundsFromFile() {
	DEBUG_PRINT("FModManager::loadSounds()\n");
	// Create a document object
	pugi::xml_document soundlibrary;
	// Load a xml file into the object
	pugi::xml_parse_result result = soundlibrary.load_file(SOUND_FILE.c_str());
	if (!result) {
		std::cout << "FModManager error: Failed to load file named #" << SOUND_FILE << std::endl;
		return;
	}
	DEBUG_PRINT("Successfully loaded file named #%s\n", SOUND_FILE.c_str());

	// Use XML object to read data from
	pugi::xml_object_range<pugi::xml_node_iterator> itSoundLibrary = soundlibrary.child("soundlibrary").children();
	if (itSoundLibrary.empty()) {
		std::cout << "FModManager error: There are no sounds in the sound library!" << std::endl;
		return;
	}

	// Parse through the tree of the XML Data using iterator
	// <soundlibrary>
	pugi::xml_node_iterator itSound;
	// Pointer to each new sound to be instanciated
	Sound* newSound;
	for (itSound = itSoundLibrary.begin(); itSound != itSoundLibrary.end(); itSound++) {
		pugi::xml_node soundNode = *itSound;
		pugi::xml_attribute category = soundNode.attribute("category");
		DEBUG_PRINT("Category: %s\n", category.value());
		if (strcmp(category.value(), "MUSIC") == 0) {		// Instanciate new sound with mode LOOP - BGM
			newSound = new Sound(FMOD_LOOP_NORMAL);
		} else if (strcmp(category.value(), "FX") == 0) {	// Instanciate new sound with mode DEFAULT - FX
			newSound = new Sound(FMOD_DEFAULT);
		} else {								// Category not defined error
			std::cout << "FModManager error: Category found on XML not defined! #" << category.value() << std::endl;
			return;
		}
		// <sound>
		pugi::xml_node_iterator itSoundInfo;
		for (itSoundInfo = soundNode.children().begin();
			itSoundInfo != soundNode.children().end();
			itSoundInfo++) {
			pugi::xml_node soundInfoNode = *itSoundInfo;
			DEBUG_PRINT(" %s: %s\n", soundInfoNode.name(), soundInfoNode.child_value());
			if (strcmp(soundInfoNode.name(), "title") == 0) {			// Sets Element named Title
				newSound->m_name = soundInfoNode.child_value();
			} else if (strcmp(soundInfoNode.name(), "path") == 0) {	// Sets Element named Path
				newSound->m_path = soundInfoNode.child_value();
			} else {										// Element not defined error
				std::cout << "FModManager error: Element found on XML not defined! #" << soundInfoNode.name() << std::endl;
			}

			// Attributes of sound - At the moment we aren't joining this loop
			for (pugi::xml_attribute_iterator itAttribute = soundInfoNode.attributes_begin();
				itAttribute != soundInfoNode.attributes_end();
				itAttribute++) {
				pugi::xml_attribute attribute = *itAttribute;
				DEBUG_PRINT("   %s: %s\n", attribute.name(), attribute.value());
			}
		}

		// Calls the FMOD function to create the new sound
		m_result = m_system->createSound(newSound->m_path.c_str(), newSound->m_mode, nullptr, &newSound->m_sound);
		// Checks the result
		if (m_result != FMOD_OK) {
			std::cout << "fmod error: #" << m_result << "-" << FMOD_ErrorString(m_result) << std::endl;
			return;
		}
		// Adds the new sound to the map tree of sounds
		m_sounds.try_emplace(newSound->m_name, newSound);
	}
}

void FModManager::playSound(const std::string& sound_name, const std::string& channel_group_name) {
	// Tries to find the sound
	std::map<std::string, Sound*>::iterator itSound = m_sounds.find(sound_name);
	// Tries to find the channel group
	std::map<std::string, ChannelGroup*>::iterator itChannel = m_channel_groups.find(channel_group_name);

	if (itSound == m_sounds.end() || itChannel == m_channel_groups.end()) {
		std::cout << "FModManager error: Couldn't find the Sound named #" << sound_name
													<< "or ChannelGroup named #" << channel_group_name << std::endl;
		return;
	}

	FMOD::Channel* channel;
	// Calls FMOD to play the sound
	m_result = m_system->playSound(itSound->second->m_sound, itChannel->second->m_group, false, &channel);
	// Checks the result
	if (m_result != FMOD_OK) {
		std::cout << "fmod error: #" << m_result << "-" << FMOD_ErrorString(m_result) << std::endl;
		return;
	}

}
