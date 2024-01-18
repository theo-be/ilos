
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <string>
#include <cstring>

#include "constants.hpp"
#include "Font.hpp"
#include "texture_management.hpp"

using namespace std;

Font::Font (const char *src, int size, SDL_Renderer *renderer) : m_renderer(renderer), m_font(nullptr), m_surface(nullptr), m_atlas(nullptr), m_size(size)
{
    m_wordBuffer = (char*)malloc(sizeof(char) * MAX_WORD_LENGTH);
    if (!m_wordBuffer) {
        cerr << "ERREUR allocation memoire MAX_WORD_LENGTH" << endl;
        exit(1);
    }
    m_lineBuffer = (char*)malloc(sizeof(char) * MAX_LINE_LENGTH);
    if (!m_lineBuffer) {
        cerr << "ERREUR allocation memoire MAX_LINE_LENGTH" << endl;
        exit(1);
    }
    m_glyphBuffer = (char*)malloc(sizeof(char) * MAX_GLYPH_SIZE);
    if (!m_glyphBuffer) {
        cerr << "ERREUR allocation memoire MAX_GLYPH_SIZE" << endl;
        exit(1);
    }
    m_glyphs = (SDL_Rect*)malloc(sizeof(SDL_Rect) * MAX_GLYPHS);
    if (!m_glyphs) {
        cerr << "ERREUR allocation memoire tableau de SDL_Rect MAX_GLYPHS" << endl;
        exit(1);
    }
    m_lineBuffer[0] = '\0';
    m_wordBuffer[0] = '\0';
    m_glyphBuffer[0] = '\0';
    init(src, size);
}

Font::~Font () {
    SDL_FreeSurface(m_surface);
    SDL_DestroyTexture(m_atlas);
    free(m_glyphs);
    free(m_wordBuffer);
    free(m_lineBuffer);
    free(m_glyphBuffer);
}

/**
 * @fn void Font::init (const char *src, int size)
 * @param src fichier source de la police
 * @param size taille initiale de la police
 * @brief initialise une police
*/
void Font::init (const char *src, int size) {

    SDL_Surface *text;
    SDL_Color white = {255, 255, 255, SDL_ALPHA_OPAQUE};
    SDL_Rect dest = {0};
    // char glyphBuffer[MAX_GLYPH_SIZE];

    // position en octet dans la chaine de caracteres, sert de curseur
    int i = 0;
    // indice dans le tableau de symboles
    int n = 0;

    // string characters = "Ö&|_# POfileorTBFS:handWCpygt2015-6,JwsbuGNUL3.Emj@c/\"IV\\RMD8+v?x;=%!AYq()'kH[]KzQX4Z79*àéí¡Çóè·úïçüºòÉÒÍÀ°æåøÆÅØ<>öÄäßÜá¿ñÁÊûâîôÈêùœÙìëęąłćżńśźŻŚŁĆÖ";

    string characters = "azertyuiopqsdfghjklmwxcvbn AZERTYUIOPQSDFGHJKLMWXCVBN0123456789&@\"'()+-*/=\\[]{}|^$#,;:!?.<>&~°éèêëçàâùûôÔîïµ";

    m_size = size;

    m_font = TTF_OpenFont(src, m_size);


    m_surface = SDL_CreateRGBSurface(0, FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE, 32, 0, 0, 0, 0xff);

    SDL_SetColorKey(m_surface, SDL_TRUE, SDL_MapRGBA(m_surface->format, 0, 0, 0, 0));

    while ((n = nextGlyph(characters.c_str(), &i, m_glyphBuffer))) {

        if (n >= MAX_GLYPHS) {
            cerr << "ERREUR : l'indice du symbole " << m_glyphBuffer << "depasse la taille du tableau dedie " << characters[i] << endl;
            exit(1);
        }

        text = TTF_RenderUTF8_Blended(m_font, m_glyphBuffer, white);

        TTF_SizeText(m_font, m_glyphBuffer, &dest.w, &dest.h);

        if (dest.x + dest.w >= FONT_TEXTURE_SIZE) {
            dest.x = 0;

            dest.y += dest.h + 1;

            if (dest.y + dest.h >= FONT_TEXTURE_SIZE) {
                // erreur

                cerr << "ERREUR : Plus de place dans l'atlas de texture, " << FONT_TEXTURE_SIZE << "x" << FONT_TEXTURE_SIZE << endl;
                exit(1);
            }
        }

        SDL_BlitSurface(text, NULL, m_surface, &dest);

        m_glyphs[n] = dest;

        SDL_FreeSurface(text);

        dest.x += dest.w;
    }

    m_atlas = toTexture(m_renderer, m_surface);
    m_surface = nullptr;
}

/**
 * @fn int Font::nextGlyph (const char *str, int *i, char *buffer)
 * @param str chaine a parcourir
 * @param i curseur octal dans la chaine
 * @param buffer conteneur pour un caractere
 * @returns l'indice dans le tableau de symboles, 0 si le caractere n'est pas pris en charge
 * @brief positionne le curseur vers le nouveau caractere et renvoie son indice dans le tableau 
*/
int Font::nextGlyph (const char *str, int *i, char *buffer) {
    // taille en octet du symbole
    int len;
    // symbole a traiter
    unsigned int bit;
    // pointeur vers le symbole a copier
    const char *p;

    bit = (unsigned char) str[*i];

    // symbole non pris en charge
    if (bit < ' ') {
        return 0;
    }

    len = 1;

    if (bit >= 0xF0) {
        bit  = (int)(str[*i]     & 0x07) << 18;
        bit |= (int)(str[*i + 1] & 0x3F) << 12;
        bit |= (int)(str[*i + 2] & 0x3F) << 6;
        bit |= (int)(str[*i + 3] & 0x3F);

        len = 4;
    }
    else if (bit >= 0xE0) {
        bit  = (int)(str[*i]     & 0x0F) << 12;
        bit |= (int)(str[*i + 1] & 0x3F) << 6;
        bit |= (int)(str[*i + 2] & 0x3F);

        len = 3;
    }
    else if (bit >= 0xC0) {
        bit  = (int)(str[*i]     & 0x1F) << 6;
        bit |= (int)(str[*i + 1] & 0x3F);

        len = 2;
    }

    if (buffer) {
        p = str + *i;
        memset(buffer, 0, MAX_GLYPH_SIZE);

        memcpy(buffer, p, len);
    }

    *i = *i + len;

    return bit;

}



/**
 * @fn void Font::clear ()
 * @brief nettoie la police
*/
void Font::clear () {
    SDL_DestroyTexture(m_atlas);
    TTF_CloseFont(m_font);
}


/**
 * @fn void Font::sizeText (const char *str, int *w, int *h)
 * @param str chaine a etudier
 * @param w longueur de la chaine
 * @param h largeur de la chaine
 * @brief calcule la taille du texte
*/
void Font::sizeText (const char *str, int *w, int *h) {
    int i = 0, n;

    *w = 0;
    *h = 0;

    while ((n = nextGlyph(str, &i))) {

        *w = *w + m_glyphs[n].w;
        *h = max<int>(*h, m_glyphs[n].h);
    }
}

/**
 * @fn void Font::displayText (const char *str, int x, int y, TextAlign alignment, SDL_Color *color)
 * @param str chaine a afficher
 * @param x position x par rapport a l'alignement
 * @param y cote haut du texte
 * @param alignment alignement du texte par rapport a x
 * @param color couleur du texte (noir si absent)
 * @brief affiche une ligne de texte
*/
void Font::displayText (const char *str, int x, int y, TextAlign alignment, SDL_Color *color) {

    int w, h;

    SDL_Rect dest = {x, y, 0};
    SDL_Rect *glyph;

    SDL_Color col;

    if (alignment != Left) {

        sizeText (str, &w, &h);


        if (alignment == Right) {
            x -= w;
        }
        else if (alignment == Center) {
            x -= w / 2;
        }
    }

    // curseur octal sur la chaine
    int i = 0;
    // indice dans le tableau de coordonees
    int n = 0;

    if (!color) 
        col = {0, 0, 0, SDL_ALPHA_OPAQUE};
    else 
        col = *color;

    SDL_SetTextureColorMod(m_atlas, col.r, col.g, col.b);


    while ((n = nextGlyph(str, &i)) != 0) {
        
        glyph = &m_glyphs[n];

        dest.x = x;
        dest.y = y;
        dest.w = glyph->w;
        dest.h = glyph->h;
        
        SDL_RenderCopy(m_renderer, m_atlas, glyph, &dest);

        x += glyph->w;  
    }
}

/**
 * @fn void Font::displayTextWrapped (const char *str, int x, int y, int mawWidth, TextAlign alignment, SDL_Color *color)
 * @param str chaine a afficher
 * @param x position x par rapport a l'alignement
 * @param y cote haut du texte
 * @param maxWidth taille maximale du texte
 * @param alignment alignement du texte par rapport a x
 * @param color couleur du texte
 * @brief affiche une ou plusieurs lignes de texte selon la taille de la chaine
*/
void Font::displayTextWrapped (const char *str, int x, int y, int maxWidth, TextAlign alignment, SDL_Color *color) {
    int i = 0, n;

    int wordWidth = 0, lineWidth = 0;

    int len = strlen(str);
    int w, h;


    m_lineBuffer[0] = m_wordBuffer[0] = m_glyphBuffer[0] = '\0';

    sizeText(str, &w, &h);
    if (w <= maxWidth) 
        displayText(str, x, y, alignment, color);

    while ((n = nextGlyph(str, &i, m_glyphBuffer))) {

        wordWidth += m_glyphs[n].w;

        if (n != ' ') {
            strcat(m_wordBuffer, m_glyphBuffer);    
        }

        if (n == ' ' || i == len) {

            if (lineWidth + wordWidth >= maxWidth) {
                displayText(m_lineBuffer, x, y, alignment, color);
                m_lineBuffer[0] = '\0';
                y += h;
                lineWidth = 0;
            } else if (lineWidth != 0) {
                strcat(m_lineBuffer, " ");
            }

            strcat(m_lineBuffer, m_wordBuffer);

            lineWidth += wordWidth;
            m_wordBuffer[0] = '\0';
            wordWidth = 0;
        }
    }

    // affichage de la derniere ligne
    displayText(m_lineBuffer, x, y, alignment, color);
}








void displayText (SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Point &center, SDL_Color &color, TextAlign alignment, bool wrap) {
    SDL_Rect dim = {0};
    TTF_SizeUTF8(font, text, &dim.w, &dim.h);
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);
    // verifier si c est bon
    SDL_Texture *texture = toTexture(renderer, surface);




    switch (alignment) {
        case Center:
            dim.x = center.x - dim.w / 2;
            dim.y = center.y - dim.h / 2;
            break;
        case Left:
            dim.x = center.x;
            dim.y = center.y;
            break;
        case Right:
            dim.x = center.x - dim.w;
            dim.y = center.y - dim.h;
            break;
        default:
            break;
    }

    SDL_RenderCopy(renderer, texture, NULL, &dim);
    SDL_DestroyTexture(texture);
}
void Font::setRenderer(SDL_Renderer *renderer) {
    m_renderer = renderer;
}

