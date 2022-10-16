#include "Sound.h"

#define DEBUG_LOG_ENABLED
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

Sound::Sound(const int mode)
	: m_sound(nullptr),
	m_name(""),
	m_path(""),
	m_mode(mode) {
	DEBUG_PRINT("Sound::Sound(%d)\n", mode);
}

Sound::Sound(const std::string& name, const std::string& path, const int mode)
	: m_sound(nullptr),
	  m_name(name),
	  m_path(path),
	  m_mode(mode) {
	DEBUG_PRINT("Sound::Sound(%s, %s, %d)\n", name.c_str(), path.c_str(), mode);
}

Sound::~Sound() {
	DEBUG_PRINT("Sound::~Sound(%s)\n", m_name.c_str());
	m_sound->release();
}
