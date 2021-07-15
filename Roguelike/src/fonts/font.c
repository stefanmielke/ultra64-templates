/*
	Example of FONTS
*/

#include <PR/mbi.h>
#include <PR/sp.h>
#include "font.h"
#include "font_ext.h"

#include "letters_img.h"

static Bitmap letters_bms[255];
static Font letters_font;
int fontcol[5][4];

// returns 0 if already finished, or read chars to call again
u32 text_sprite_ex(Sprite *txt, ConsoleChar *str, Font *fnt, int *start, int width) {
	txt->height = FONT_HEIGHT;
	Bitmap *bm = txt->bitmap;

	u8 current_color = str[(*start)].color;
	// check color and change
	switch (current_color) {
		case CONSOLE_COLOR_1:
			font_set_color(fontcol[0][0], fontcol[0][1], fontcol[0][2], fontcol[0][3]);
			break;
		case CONSOLE_COLOR_2:
			font_set_color(fontcol[1][0], fontcol[1][1], fontcol[1][2], fontcol[1][3]);
			break;
		case CONSOLE_COLOR_3:
			font_set_color(fontcol[2][0], fontcol[2][1], fontcol[2][2], fontcol[2][3]);
			break;
		case CONSOLE_COLOR_4:
			font_set_color(fontcol[3][0], fontcol[3][1], fontcol[3][2], fontcol[3][3]);
			break;
		case CONSOLE_COLOR_5:
			font_set_color(fontcol[4][0], fontcol[4][1], fontcol[4][2], fontcol[4][3]);
			break;
		default:
			break;
	}

	int pos = 0;
	for (int i = 0; i < width; i++) {
		if (i > 0 && str[i + (*start)].color != current_color) {
			txt->nbitmaps = pos;
			txt->width = pos * FONT_WIDTH;
			(*start) += pos;
			return pos;
		}

		bm[pos] = fnt->bitmaps[str[i + (*start)].character];
		pos++;
	}

	txt->width = pos * FONT_WIDTH;
	txt->nbitmaps = pos;
	(*start) += pos;
	return pos;
}

void text_sprite(Sprite *txt, char *str, Font *fnt, int xlen, int ylen) {
	int i, ci;
	int x;
	int y;
	Bitmap *bm;

	txt->width = xlen * FONT_WIDTH;
	txt->height = ylen * FONT_HEIGHT;

	bm = txt->bitmap;

	i = 0;
	ci = 0;
	for (y = 0; y < ylen; y++) {
		for (x = 0; x < xlen; x++, i++, ci++) {
			if (str[ci] == '\0') {
				bm[i] = fnt->bitmaps[0];
				bm[i].width = -1;
				txt->nbitmaps = i;
				return;
			}
			if (str[ci] == '\n') {
				bm[i] = fnt->bitmaps[0];
				bm[i].buf = NULL;
				ci--;
				continue;
			}

			bm[i] = fnt->bitmaps[str[ci]];
		};

		if (str[ci] == '\n')
			ci++;
	};

	txt->nbitmaps = i;

	return;
}

#define DYN_SPRITE_HACK

#define NUM_template_BMS (255)

static Bitmap template_bm[NUM_template_BMS];

static Gfx template_dl[NUM_DL(NUM_template_BMS)];

Sprite template_sprite = {

	0,
	0, /* Position: x,y */

	0,
	0, /* Sprite size in texels (x,y) */

	1.0,
	1.0, /* Sprite Scale: x,y */

	0,
	0, /* Explosion (x,y) */

	SP_TRANSPARENT, /* Sprite Attributes */
	0x1234,			/* Sprite Depth: Z */

	255,
	255,
	255,
	255, /* Sprite Coloration: RGBA */

	0,
	0,
	NULL, /* Color LookUp Table: start_index, length, address */

	0,
	1, /* Sprite Bitmap index: start index, step increment */

	NUM_template_BMS,		  /* Number of bitmaps */
	NUM_DL(NUM_template_BMS), /* Number of display list locations allocated */

	FONT_HEIGHT - 1,
	SPRITE_WIDTH, /* Sprite Bitmap Height: Used_height, physical height */
	G_IM_FMT_I,	  /* Sprite Bitmap Format */
	G_IM_SIZ_4b,  /* Sprite Bitmap Texel Size */

	template_bm, /* Pointer to bitmaps */

	template_dl, /* Display list memory */

	NULL, /* HACK: dynamic_dl pointer */

};

static unsigned char font_red = 255;
static unsigned char font_grn = 255;
static unsigned char font_blu = 255;
static unsigned char font_alf = 255;

static int font_xpos = 0;
static int font_ypos = 0;

static int font_win_width = 40;

static double font_xscale = 1.0;
static double font_yscale = 1.0;

void font_setup() {
	for (int i = 0; i < 5; ++i) {
		fontcol[i][0] = 255;
		fontcol[i][1] = 255;
		fontcol[i][2] = 255;
		fontcol[i][3] = 255;
	}

	for (unsigned char i = 0; i < 255; ++i) {
		letters_bms[i].width = FONT_WIDTH;
		letters_bms[i].width_img = SPRITE_WIDTH;
		letters_bms[i].s = (i % SPRITE_CHAR_COUNT_X) * FONT_WIDTH;
		letters_bms[i].t = (i / SPRITE_CHAR_COUNT_Y) * FONT_HEIGHT;
		letters_bms[i].buf = letters_img;
		letters_bms[i].actualHeight = FONT_HEIGHT;
		letters_bms[i].LUToffset = 0;
	}

	letters_font.bitmaps = letters_bms;
	letters_font.img = letters_img;
}

/* Initialize Font local variables and prepare for drawing sprites */

void font_init(Gfx **glistp) {
	Gfx *gxp;

	gxp = *glistp;

	spInit(&gxp);

	template_sprite.rsp_dl_next = template_sprite.rsp_dl;

	*glistp = gxp;

	font_red = 255;
	font_grn = 255;
	font_blu = 255;
	font_alf = 255;

	font_xpos = 0;
	font_ypos = 0;

	font_win_width = 40;

	font_xscale = 1.0;
	font_yscale = 1.0;
}

/* Call spFinish() to clean up when done using Sprites */

void font_finish(Gfx **glistp) {
	Gfx *gxp;

	gxp = *glistp;

	spFinish(&gxp);

	*glistp = (gxp - 1); /* Don't use final EndDisplayList() */
}

/* Set text window area (units are characters) */

void font_set_win(int width) {
	font_win_width = width;
}

/* Set text window position (upper left corner) */

void font_set_pos(int xpos, int ypos) {
	font_xpos = xpos;
	font_ypos = ypos;
}

/* Set text size */

void font_set_scale(double xscale, double yscale) {
	font_xscale = xscale;
	font_yscale = yscale;
}

/* Set text color */

void font_set_internal_color(char color, int r, int g, int b, int a) {
	switch (color) {
		case CONSOLE_COLOR_1:
			fontcol[0][0] = r;
			fontcol[0][1] = g;
			fontcol[0][2] = b;
			fontcol[0][3] = a;
			break;
		case CONSOLE_COLOR_2:
			fontcol[1][0] = r;
			fontcol[1][1] = g;
			fontcol[1][2] = b;
			fontcol[1][3] = a;
			break;
		case CONSOLE_COLOR_3:
			fontcol[2][0] = r;
			fontcol[2][1] = g;
			fontcol[2][2] = b;
			fontcol[2][3] = a;
			break;
		case CONSOLE_COLOR_4:
			fontcol[3][0] = r;
			fontcol[3][1] = g;
			fontcol[3][2] = b;
			fontcol[3][3] = a;
			break;
		case CONSOLE_COLOR_5:
			fontcol[4][0] = r;
			fontcol[4][1] = g;
			fontcol[4][2] = b;
			fontcol[4][3] = a;
			break;
		default:
			break;
	}
}

void font_set_color(unsigned char red, unsigned char green, unsigned char blue,
					unsigned char alpha) {
	font_red = red;
	font_grn = green;
	font_blu = blue;
	font_alf = alpha;
}

void font_set_transparent(int flag) {
	if (flag)
		spSetAttribute(&template_sprite, SP_TRANSPARENT | SP_CUTOUT);
	else
		spClearAttribute(&template_sprite, SP_TRANSPARENT | SP_CUTOUT);
}

/* Convert the string to a sprite with the propler bitmaps
   assembled from the basic font texture. */

void font_show_string(Gfx **glistp, char *val_str) {
	Sprite *sp;
	static Gfx gx[20000];
	Gfx *gxp, *dl;

	gxp = *glistp;

	sp = &template_sprite;

	sp->width = font_win_width * FONT_WIDTH + FONT_WIDTH;
	sp->height = FONT_HEIGHT + FONT_HEIGHT;

	text_sprite(sp, val_str, &letters_font, font_win_width, 1);

	spMove(sp, font_xpos, font_ypos);
	spColor(sp, font_red, font_grn, font_blu, font_alf);
	spScale(sp, font_xscale, font_yscale);

	dl = spDraw(sp);

	gSPDisplayList(gxp++, dl);

	*glistp = gxp;
}

void font_show_string_ex(Gfx **glistp, ConsoleChar *val_str) {
	Sprite *sp;
	static Gfx gx[20000];
	Gfx *gxp, *dl;

	gxp = *glistp;

	sp = &template_sprite;

	// text_sprite(sp, val_str, &letters_font, font_win_width, 1);
	u32 chars_read = 0;
	u32 left = font_win_width;
	int next = 0;
	do {
		u32 x_pos = font_win_width - left;

		chars_read = text_sprite_ex(sp, val_str, &letters_font, &next, left);
		if (chars_read == 0)
			break;
		left -= chars_read;

		spMove(sp, font_xpos + (x_pos * FONT_WIDTH), font_ypos);
		spColor(sp, font_red, font_grn, font_blu, font_alf);
		spScale(sp, font_xscale, font_yscale);

		dl = spDraw(sp);

		gSPDisplayList(gxp++, dl);
	} while (left > 0);

	*glistp = gxp;
}
