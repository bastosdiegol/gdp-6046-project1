#pragma once
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>

class FModManager {
public:
	const int MAX_CHANNELS = 255;

	// Constructor
	FModManager(const int system_flags);
	// Destructor
	~FModManager();
protected:
	FMOD_RESULT		m_result;
	FMOD::System*	m_system;
};

