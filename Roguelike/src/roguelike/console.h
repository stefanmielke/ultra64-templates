#pragma once

#include <ultra64.h>

#define CONSOLE_SIZE_X 48
#define CONSOLE_SIZE_Y 31

/**
 * @brief Initializes the console
 */
void console_init();

/**
 * @brief Set one of the colors available to the color passed.
 *
 * @param color
 *        Color to use. One of CONSOLE_COLOR_1, CONSOLE_COLOR_3, CONSOLE_COLOR_3, CONSOLE_COLOR_4,
 * CONSOLE_COLOR_5.
 * @param r
 *        Red value of the color.
 * @param g
 *        Green value of the color.
 * @param b
 *        Blue value of the color.
 * @param a
 *        Alpha value of the color.
 */
void console_set_color(char color, int r, int g, int b, int a);

/**
 * @brief Print a text to the console. Use \1..5 in the text to set the color - eg.:
 * console_print(0,0, "Hello \1World", 11)
 *
 * @param x
 *        X location in the console.
 * @param y
 *        Y location in the console.
 * @param text
 *        Text to print.
 * @param text_length
 *        Length of the text (without colors).
 */
void console_print(u8 x, u8 y, u8 *text, u8 text_length);

/**
 * @brief Clear all the console to NULL.
 */
void console_clear();

/**
 * @brief Draw the console to the display list.
 *
 * @param glistp
 *        Display list to use.
 */
void console_draw(Gfx **glistp);
