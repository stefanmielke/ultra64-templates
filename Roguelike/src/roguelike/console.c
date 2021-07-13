#include "console.h"

#include "../fonts/font_ext.h"

#define CONSOLE_SIZE_X 48
#define CONSOLE_SIZE_Y 31

u8 console[CONSOLE_SIZE_Y][CONSOLE_SIZE_X + 1];

int fontcol[4]; /* color for shadowed fonts */
#define FONT_COL 255, 255, 255, 255
#define FONT_COL_ENEMY 255, 0, 0, 255
#define FONTCOL(r, g, b, a)                                                                        \
	{                                                                                              \
		fontcol[0] = r;                                                                            \
		fontcol[1] = g;                                                                            \
		fontcol[2] = b;                                                                            \
		fontcol[3] = a;                                                                            \
	}
#define FONTCOLM(c) FONTCOL(c)
#define SHOWFONT(glp, str, x, y)                                                                   \
	{                                                                                              \
		font_set_color(0, 0, 0, 255);                                                              \
		font_set_pos((x) + (1), (y) + (0));                                                        \
		font_show_string(glp, str);                                                                \
		font_set_pos((x) + (0), (y) + (1));                                                        \
		font_show_string(glp, str);                                                                \
		font_set_color(fontcol[0], fontcol[1], fontcol[2], fontcol[3]);                            \
		font_set_pos(x, y);                                                                        \
		font_show_string(glp, str);                                                                \
	}

void console_init() {
	font_setup();

	console_clear();
}

void console_clear() {
	for (u8 y = 0; y < CONSOLE_SIZE_Y; ++y) {
		for (u8 x = 0; x < CONSOLE_SIZE_X; ++x) {
			console[y][x] = '#';
		}
		console[y][CONSOLE_SIZE_X] = '\0';
	}
}

void console_print(u8 x, u8 y, char *text, u8 text_length) {
	for (u8 i = 0; i < text_length || x > CONSOLE_SIZE_X; i++) {
		console[y][x] = text[i];
		x++;
	}
}

void console_draw(Gfx **glistp) {
	font_init(glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(CONSOLE_SIZE_X, 1);
	FONTCOLM(FONT_COL);

	for (u8 y = 0; y < CONSOLE_SIZE_Y; ++y) {
		SHOWFONT(glistp, &console[y][0], 15, y * 7 + 10);
	}

	font_finish(glistp);
}
