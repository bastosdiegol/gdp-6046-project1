#pragma once
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>
#include <string>

class Sound {
public:
	FMOD::Sound*	m_sound;
	std::string		m_name;
	std::string		m_path;
	const int		m_mode;

	// Constructor
	// Accepts only the mode all other member variables are null
	Sound(const int mode);
	// Constructor
	// Accepts name, path and mode. FMOD sound is null
	Sound(const std::string& name, const std::string& path, const int mode);
	// Destructor
	~Sound();
};

