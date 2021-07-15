#pragma once

#define CONSOLE_COLOR_1 '\1'
#define CONSOLE_COLOR_2 '\2'
#define CONSOLE_COLOR_3 '\3'
#define CONSOLE_COLOR_4 '\4'
#define CONSOLE_COLOR_5 '\5'

typedef struct {
	u8 character;
	u8 color;
} ConsoleChar;

void font_setup();
void font_init(Gfx **);
void font_finish(Gfx **);
void font_show_num(Gfx **, int);
void font_show_string(Gfx **, char *);
void font_show_string_ex(Gfx **, ConsoleChar *text);

void font_set_scale(double, double);
void font_set_color(unsigned char, unsigned char, unsigned char, unsigned char);
void font_set_internal_color(char color, int r, int g, int b, int a);
void font_set_win(int);
void font_set_pos(int, int);

void font_set_transparent(int);
