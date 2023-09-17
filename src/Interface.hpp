
#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL.h>

#include <iostream>
#include <deque>

#include "TextBox.hpp"
#include "Scene.hpp"
#include "Entity.hpp"
#include "Player.hpp"


class Interface {
    public:
    Interface(SDL_Rect dimension);

    ~Interface ();

    void displayInterface (SDL_Renderer *renderer, Scene &scene, Entity &entity, TextBox &messageBox) const;

    SDL_Texture *getTexture (int id) const;

    private:
    SDL_Rect dimension;
    std::deque<SDL_Texture*> fontTextures;

};





#endif