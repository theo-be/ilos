#ifndef FONT_H
#define FONT_H

#include <SDL2/SDL.h>

#include "constants.hpp"

class Font {
    public:
    Font (const char *src, int size, SDL_Renderer *renderer);
    ~Font ();
    void init (const char *src, int size);
    void clear ();
    void displayText (const char *str, int x, int y, TextAlign alignment = Left, SDL_Color *color = nullptr);
    void displayTextWrapped (const char *str, int x, int y, int maxWidth, TextAlign alignment = Left, SDL_Color *color = nullptr);
    // void displayBoxedText (SDL_Renderer *renderer, const char *str, const SDL_Rect &box, TextAlign alignment = Left, SDL_Color *color = nullptr); 
    // Arranger la taille du texte pour qu'il rentre dans la boite


    private:
    int nextGlyph (const char *str, int *i, char *buffer = NULL);
    void sizeText (const char *str, int *w, int *h);


    SDL_Renderer *m_renderer;
    TTF_Font *m_font;
    SDL_Surface *m_surface;
    SDL_Texture *m_atlas;
    int m_size;
    SDL_Rect *m_glyphs;

    char *m_wordBuffer;
    char *m_lineBuffer;
    char *m_glyphBuffer;
};



void displayText (SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Point &center, SDL_Color &color, TextAlign alignment = Center, bool wrap = 0);








#endif