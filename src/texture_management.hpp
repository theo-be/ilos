#ifndef TEXTURES_MANAGEMENT_H
#define TEXTURES_MANAGEMENT_H

#include <SDL.h>

#include <string>


SDL_Texture *loadTexture(SDL_Renderer *renderer, const std::string &fileName);

SDL_Texture *toTexture (SDL_Renderer *renderer, SDL_Surface *surface);





#endif