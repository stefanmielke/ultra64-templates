#include <nusys.h>

#include "../graphic.h"
#include "../definitions.h"

#include "../roguelike/console.h"
#include "../roguelike/player.h"
#include "../roguelike/sample_map.h"

Player player;

void clear_background(u8 r, u8 g, u8 b);

void stage00_init(void) {
	console_init();

	player_init(&player, 1, 1);
}

void stage00_update(void) {
	console_clear();

	for (u8 y = 0; y < CONSOLE_SIZE_Y; ++y) {
		console_print(0, y, &sample_map[y * CONSOLE_SIZE_X], CONSOLE_SIZE_X);
	}

	console_print(0, 0, "Hello World", 11);

	if (ButtonWasPressed(0, U_JPAD)) {
		player_move(&player, 0, -1);
	} else if (ButtonWasPressed(0, D_JPAD)) {
		player_move(&player, 0, 1);
	} else if (ButtonWasPressed(0, L_JPAD)) {
		player_move(&player, -1, 0);
	} else if (ButtonWasPressed(0, R_JPAD)) {
		player_move(&player, 1, 0);
	}
	console_print(player.x, player.y, "@", 1);
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
	gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, nuGfxCfb_ptr);
	gDPSetFillColor(glistp++, (GPACK_RGBA5551(r, g, b, 1) << 16 | GPACK_RGBA5551(r, g, b, 1)));
	gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
	gDPPipeSync(glistp++);
}
