

#include <vector>
#include <iterator>
#include <string>

#include "Debug.hpp"
#include "App.hpp"

#include "DebugInfo.hpp"

using namespace std;



// unordered_map<string, vector<string>> Debug::m_debugSections;
// unordered_map<string, DebugInfo> Debug::m_debugSections;

Debug::Debug() {

}

Debug::~Debug() {

}


void Debug::addLabel(const char *name) {
    
}

void Debug::addInfo (const string &label, const string &info) {

    static string i = "";
    // m_debugSections[label + i].push_back();
    i.append(label);

}

void Debug::update() {
    if (isPressed(SDLK_p))
        m_gameLaunched = false;
    if (isPressed(SDLK_ESCAPE))
        m_gamePaused = !m_gamePaused;

    if (isPressed(SDLK_o)) {
        // string label = "section";
        // string info = "info";
        // addInfo(label, info);
    }

    if (isPressed(SDLK_m)) {
        m_camera->centerToTarget();
    }
}


void Debug::displayInfos () {

    // int startX = 0, startY = 0;
    // int offsetX = 0, offsetY = 0;
    
    // for (auto it = m_debugSections.begin(); it != m_debugSections.end(); it++) {
    //     m_font->displayText((*it).first.c_str(), startX + offsetX, startY + offsetY);
    //     offsetY+= 20;
    // }
}


/* 
player
touches de debug
dt
fix police
charger le joueur depuis un fichier
 */