#ifndef CONSTANTS_H
#define CONSTANTS_H

#define WINDOWS

#define WINDOW_WIDTH 1280 // 1920
#define WINDOW_HEIGHT 720 // 1080

// millisecondes
#define F_DELTA 16

// microsecondes
#define FRAME_DELTA 16666

#define TILE_SIZE 32

#define TILES_TEXTURE_COUNT 4
#define ENTITY_TEXTURE_COUNT 4

#define SOLID_TILES_COUNT 2
#define TRANSPARENT_TILES_COUNT 2

#define DEFAULT_MAP_WIDTH 1000
#define DEFAULT_MAP_HEIGHT 1000

#define MAP_RENDER_WIDTH (WINDOW_WIDTH / TILE_SIZE + 1)
#define MAP_RENDER_HEIGHT (WINDOW_HEIGHT / TILE_SIZE + 1)

// zone pour la camera
#define MAX_AREA_WIDTH (3 * MAP_RENDER_WIDTH * TILE_SIZE / 4)
#define MAX_AREA_HEIGHT (2 * MAP_RENDER_HEIGHT * TILE_SIZE / 3)

#define FONT_SIZE_INTERFACE 16
#define FONT_TEXTURE_SIZE 600

#define MAX_GLYPHS 400

// en octet
#define MAX_GLYPH_SIZE 8
#define MAX_WORD_LENGTH 31
#define MAX_LINE_LENGTH 151




#define DEFAULT_DIALOGUE_OFFSET 20

// enumerations

enum TextAlign {Left, Right, Center, Justify};

enum InteractionType {None, Talk};

// Converti en classes enfantes de Entity
// enum EntityType {Mob, Block, ItemDrop};




// vieux 
#define TEXT_ALIGN_LEFT 1
#define TEXT_ALIGN_RIGHT 2
#define TEXT_ALIGN_CENTER 3
#define TEXT_ALIGN_JUSTIFY 4


#define FONT_LETTER_COUNT 26 //
#define FONT_NUMBER_COUNT 10 //
#define FONT_TEXTURE_COUNT (FONT_LETTER_COUNT + FONT_NUMBER_COUNT) //


#define MAX_FILENAME_LENGTH 50

#define DEFAULT_PLAYER_MOVING_SPEED 400

#endif