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
	if(m_result != FMOD_OK)
		std::cout << "fmod error: #" << m_result << "-" << FMOD_ErrorString(m_result) << std::endl;

	// Initialize the System
	m_result = m_system->init(MAX_CHANNELS, system_flags, nullptr);
	// Checks for errors
	if (m_result != FMOD_OK)
		std::cout << "fmod error: #" << m_result << "-" << FMOD_ErrorString(m_result) << std::endl;
}

FModManager::~FModManager() {
	DEBUG_PRINT("FModManager::~FModManager()\n");
	m_system->release();
}
