#include "Stream.h"

Stream::Stream() {
	m_openState = FMOD_OPENSTATE_READY;
	m_percentage = 0;
	m_isStarving = false;
}

Stream::~Stream() {
	m_sound->release();
}

std::string Stream::getState() {
	switch (m_openState) {
	case FMOD_OPENSTATE_READY:
		return "Ready to play.";
	case FMOD_OPENSTATE_LOADING:
		return "Loading...";
	case FMOD_OPENSTATE_ERROR:
		return "Error.";
	case FMOD_OPENSTATE_CONNECTING:
		return "Connecting...";
	case FMOD_OPENSTATE_BUFFERING:
		return "Buffering...";
	case FMOD_OPENSTATE_SEEKING:
		return "Seeking...";
	case FMOD_OPENSTATE_PLAYING:
		return "Playing.";
	case FMOD_OPENSTATE_SETPOSITION:
		break;
	case FMOD_OPENSTATE_MAX:
		break;
	case FMOD_OPENSTATE_FORCEINT:
		break;
	default:
		return "Invalid State.";
		break;
	}
}
