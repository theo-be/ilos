

#include "Debug.hpp"
#include "App.hpp"




Debug::Debug() {

}

Debug::~Debug() {

}

void Debug::update() {
    if (isPressed(SDLK_p))
        m_gameLaunched = false;
}


