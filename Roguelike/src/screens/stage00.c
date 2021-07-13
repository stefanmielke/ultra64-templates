#include <nusys.h>

#include "../graphic.h"
#include "../definitions.h"
#include "../fonts/font_ext.h"

// start font
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
// end font

void clear_background(u8 r, u8 g, u8 b);

void stage00_init(void) {
	font_setup();
}

void stage00_update(void) {
}

void stage00_draw(void) {
	glistp = glist;
	rcp_init(glistp);

	clear_background(21, 22, 29);

	font_init(&glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(200, 1);
	FONTCOLM(FONT_COL);

	// 48 x 31
	for (int i = 0; i < 31; ++i) {
		SHOWFONT(&glistp, "################################################", 15, i * 7 + 10);
	}

	font_finish(&glistp);

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
	nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx), NU_GFX_UCODE_F3DEX2,
				   NU_SC_SWAPBUFFER);
}

void clear_background(u8 r, u8 g, u8 b) {
	gDPPipeSync(glistp++);
	gDPSetCycleType(glistp++, G_CYC_FILL);
	gDPSetDepthImage(glistp++, nuGfxZBuffer);
	gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, nuGfxZBuffer);
	gDPSetFillColor(glistp++, (GPACK_ZDZ(G_MAXFBZ, 0) << 16 | GPACK_ZDZ(G_MAXFBZ, 0)));
	gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
	gDPPipeSync(glistp++);
	gDPSetCycleType(glistp++, G_CYC_FILL);
	gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, nuGfxCfb_ptr);
	gDPSetFillColor(glistp++, (GPACK_RGBA5551(r, g, b, 1) << 16 | GPACK_RGBA5551(r, g, b, 1)));
	gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
	gDPPipeSync(glistp++);
}
