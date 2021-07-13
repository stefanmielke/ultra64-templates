#include <nusys.h>

#include "../roguelike/console.h"
#include "../graphic.h"
#include "../definitions.h"

void clear_background(u8 r, u8 g, u8 b);

void stage00_init(void) {
	console_init();
}

void stage00_update(void) {
	console_clear();
	console_print(1, 1, "Hello World", 11);
}

void stage00_draw(void) {
	glistp = glist;
	rcp_init(glistp);

	clear_background(21, 22, 29);

	console_draw(&glistp);

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
