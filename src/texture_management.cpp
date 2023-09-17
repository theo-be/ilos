#include <SDL.h>

#include <iostream>
#include <fstream>
#include <string>

#include "texture_management.hpp"

using namespace std;


/**
 * @fn SDL_Texture *loadTexture (SDL_Renderer *renderer, const string &fileName)
 * @param renderer renderer
 * @param fileName nom du fichier source
 * @returns la texture
 * @brief charge une texture
*/
SDL_Texture *loadTexture (SDL_Renderer *renderer, const string &fileName) {
    SDL_Surface *surface;

    surface = SDL_LoadBMP(fileName.c_str());
    if (!surface) {
        cerr << "Erreur de chargement de fichier : " << fileName << ", " << SDL_GetError() << endl;
        return nullptr;
    }
    return toTexture(renderer, surface);
}

/**
 * @fn SDL_Texture *toTexture (SDL_Renderer *renderer, SDL_Surface *surface)
 * @param renderer renderer
 * @param surface surface
 * @returns la texture
 * @brief transforme une surface en texture et libere la surface
*/
SDL_Texture *toTexture (SDL_Renderer *renderer, SDL_Surface *surface) {
    SDL_Texture *texture;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        cerr << "Erreur de creation de texture, " << SDL_GetError() << endl;
        return nullptr;
    }
    return texture;
}