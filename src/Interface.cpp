
#include <SDL.h>

#include <iostream>
#include <deque>
#include <cmath>

#include "Interface.hpp"
#include "TextBox.hpp"
#include "Scene.hpp"
#include "Entity.hpp"
#include "Player.hpp"

Interface::Interface (SDL_Rect dimension) : dimension(dimension)
{}

Interface::~Interface () {
    fontTextures.clear();
}

void Interface::displayInterface (SDL_Renderer *renderer, Scene &scene, Entity &entity, TextBox &messageBox) const {
    if (messageBox.isUsed()) {
        
    }
}

SDL_Texture *Interface::getTexture (int id) const {
    return fontTextures.at(id);
}