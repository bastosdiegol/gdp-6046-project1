#pragma once
#include "Sound.h"
#include <FMOD/fmod.hpp>
#include <vector>

class Stream : public Sound {
public:
    FMOD_OPENSTATE m_openState;
    unsigned int m_percentage;
    bool m_isStarving;

    std::vector<std::string> v_tags;

    // Default Constructor
    Stream();
    // Destructor
    ~Stream();

    std::string getState();
};

