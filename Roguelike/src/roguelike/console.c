#include "console.h"

#include "../fonts/font_ext.h"

ConsoleChar console[CONSOLE_SIZE_Y][CONSOLE_SIZE_X];

void console_init() {
	font_setup();

	console_clear();
}

void console_set_color(char color, int r, int g, int b, int a) {
	font_set_internal_color(color, r, g, b, a);
}

void console_clear() {
	for (u8 y = 0; y < CONSOLE_SIZE_Y; ++y) {
		for (u8 x = 0; x < CONSOLE_SIZE_X; ++x) {
			console[y][x].character = '\0';
			console[y][x].color = CONSOLE_COLOR_1;
		}
	}
}

void console_print(u8 x, u8 y, char *text, u8 text_length) {
	u8 current_color = CONSOLE_COLOR_1;
	for (u8 i = 0; i < text_length && x < CONSOLE_SIZE_X; i++) {
		switch (text[i]) {
			case CONSOLE_COLOR_1:
			case CONSOLE_COLOR_2:
			case CONSOLE_COLOR_3:
			case CONSOLE_COLOR_4:
			case CONSOLE_COLOR_5:
				console[y][x].character = text[i + 1];
				console[y][x].color = text[i];
				current_color = console[y][x].color;
				i++;
				break;
			default:
				console[y][x].character = text[i];
				console[y][x].color = current_color;
				break;
		}

		x++;
	}
}

void console_draw(Gfx **glistp) {
	font_init(glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(CONSOLE_SIZE_X);

	font_set_color(255, 255, 255, 255);

	for (u8 y = 0; y < CONSOLE_SIZE_Y; ++y) {
		font_set_pos(15, y * 7 + 10);
		font_show_string(glistp, &console[y][0]);
	}

	font_finish(glistp);
}
