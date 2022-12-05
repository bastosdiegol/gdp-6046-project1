#pragma once
#include "Sound.h"
#include <FMOD/fmod.hpp>
#include <map>

class Stream : public Sound {
public:
    FMOD_OPENSTATE m_openState;
    unsigned int m_percentage;
    bool m_isStarving;

    std::map<std::string, std::string> v_tags;

    // Default Constructor
    Stream();
    // Destructor
    ~Stream();

    std::string getState();
};

