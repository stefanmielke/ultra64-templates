/*
	Fonts: Definitions and externals.
*/

#include <PR/sp.h>

typedef struct {
	Bitmap *bitmaps;
	char *img;
} Font;

// Width of a character
#define FONT_WIDTH 6
// Height of a character
#define FONT_HEIGHT 6
// Total width of the font sprite
#define SPRITE_WIDTH 96
// Amount of characters per row
#define SPRITE_CHAR_COUNT_X 16
// Amount of characters per column
#define SPRITE_CHAR_COUNT_Y 16
