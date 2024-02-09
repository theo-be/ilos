#ifndef FONT_H
#define FONT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>



#define MAX_GLYPHS 400

// en octets
#define MAX_GLYPH_SIZE 8
#define MAX_WORD_LENGTH 31
#define MAX_LINE_LENGTH 151

#define FONT_TEXTURE_SIZE 600

#define DEFAULT_FONT_LOCATION "resources/fonts/16020_FUTURAM.ttf"
#define DEFAULT_FONT_SIZE 16


enum TextAlign {Left, Right, Center, Justify};

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

    void setRenderer(SDL_Renderer *renderer);

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