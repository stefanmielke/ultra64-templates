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

void text_sprite(Sprite *txt, char *str, Font *fnt, int xlen, int ylen) {
	int i, ci;
	int x;
	int y;
	char *indx;
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
static int font_win_height = 1;

static double font_xscale = 1.0;
static double font_yscale = 1.0;

void font_setup() {
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
	font_win_height = 1;

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

void font_set_win(int width, int height) {
	font_win_width = width;
	font_win_height = height;
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
	sp->height = font_win_height * FONT_HEIGHT + FONT_HEIGHT;

	text_sprite(sp, val_str, &letters_font, font_win_width, font_win_height);

	spMove(sp, font_xpos, font_ypos);
	spColor(sp, font_red, font_grn, font_blu, font_alf);
	spScale(sp, font_xscale, font_yscale);

	dl = spDraw(sp);

	gSPDisplayList(gxp++, dl);

	*glistp = gxp;
}
