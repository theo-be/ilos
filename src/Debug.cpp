

#include <vector>
#include "Debug.hpp"
#include "App.hpp"

#include "DebugInfo.hpp"

using namespace std;



vector<DebugInfo> Debug::m_debugSections;

Debug::Debug() {

}

Debug::~Debug() {

}


void Debug::addLabel(const char *name) {
    
}

void Debug::addInfo (const char *label, const string &info) {
    
}

void Debug::update() {
    if (isPressed(SDLK_p))
        m_gameLaunched = false;
}


