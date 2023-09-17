#ifndef ENTITIES_H
#define ENTITIES_H

#include <SDL2/SDL.h>

#include <list>
#include <deque>

#include "Entity.hpp"
#include "texture_management.hpp"
#include "Player.hpp"

void showEntityList (std::list<Entity> *list);

void killEntity (std::list<Entity> &list, Entity &e);

int loadEntityTextures (SDL_Renderer *renderer, std::deque<SDL_Texture*> &entityTextures);

void unloadEntityTextures (std::deque<SDL_Texture*> &entityTextures);

void addEntityToPlayerPos (std::list<Entity> &entityList, Player &player);


bool checkEntityCollision (std::list<Entity> &list, Entity &target);



// inutilise
int loadEntities (std::list<Entity> &entityList);
#endif