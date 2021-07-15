#include <nusys.h>
#include <stdbool.h>

#include "../graphic.h"
#include "../definitions.h"

#include "../fonts/font_ext.h"
#include "../roguelike/console.h"
#include "../roguelike/player.h"
#include "../roguelike/sample_map.h"

// player data
Player player;
// used to only redraw the screen if there was player interaction (see 'stage00_update')
bool should_redraw;

// clear background to the colors sent
void clear_background(u8 r, u8 g, u8 b);

// called after initializing the game (only once)
void stage00_init(void) {
	// initializes the console
	console_init();

	// setting default color as WHITE
	console_set_color(CONSOLE_COLOR_1, 255, 255, 255, 255);

	// setting second color as RED. We'll use this color to draw the player on 'stage00_update'
	console_set_color(CONSOLE_COLOR_2, 255, 0, 0, 255);

	// setting third color as BLUE. We'll use this color to draw "Hello World" on 'stage00_update'
	console_set_color(CONSOLE_COLOR_3, 0, 0, 255, 255);

	// initializes the player at 1,1 (map starts at 0,0)
	player_init(&player, 1, 1);

	// first frame needs a redraw
	should_redraw = true;
}

// called every frame to update the game state
void stage00_update(void) {
	// player movement. right now the only code that will force a redraw
	if (ButtonWasPressed(0, U_JPAD)) {
		player_move(&player, 0, -1);
		should_redraw = true;
	} else if (ButtonWasPressed(0, D_JPAD)) {
		player_move(&player, 0, 1);
		should_redraw = true;
	} else if (ButtonWasPressed(0, L_JPAD)) {
		player_move(&player, -1, 0);
		should_redraw = true;
	} else if (ButtonWasPressed(0, R_JPAD)) {
		player_move(&player, 1, 0);
		should_redraw = true;
	}

	if (should_redraw) {
		// clear the console buffer (sets everything to wall: '#')
		console_clear();

		// print the sample map (ideally you should draw what's on the camera)
		for (u8 y = 0; y < CONSOLE_SIZE_Y; ++y) {
			console_print(0, y, &sample_map[y * CONSOLE_SIZE_X], CONSOLE_SIZE_X);
		}

		// overwriting what's on the map with "Hello World"
		console_print(0, 1, "\3Hello World", 12);
		// draw the player on its position in red (\2 is the second color that we set on
		// 'stage00_init')
		console_print(player.x, player.y, "\2@", 2);
	}
}

// called every frame to draw to the screen. will only draw if 'should_draw' is 'true'.
void stage00_draw(void) {
	if (should_redraw) {
		should_redraw = false;

		glistp = glist;
		rcp_init(glistp);

		// clear background with a black color
		clear_background(0, 0, 0);

		// draw the console that was set on 'stage00_update'
		console_draw(&glistp);

		// end drawing and send data to RDP
		gDPFullSync(glistp++);
		gSPEndDisplayList(glistp++);
		nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx), NU_GFX_UCODE_F3DEX2,
					   NU_SC_SWAPBUFFER);
	}
}

// clear background to the colors sent
void clear_background(u8 r, u8 g, u8 b) {
	gDPPipeSync(glistp++);
	gDPSetCycleType(glistp++, G_CYC_FILL);
	gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, nuGfxCfb_ptr);
	gDPSetFillColor(glistp++, (GPACK_RGBA5551(r, g, b, 1) << 16 | GPACK_RGBA5551(r, g, b, 1)));
	gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
	gDPPipeSync(glistp++);
}
