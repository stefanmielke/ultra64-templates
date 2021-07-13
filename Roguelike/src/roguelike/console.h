#pragma once

#include <ultra64.h>

void console_init();
void console_print(u8 x, u8 y, char *text, u8 text_length);
void console_clear();
void console_draw(Gfx **glistp);
