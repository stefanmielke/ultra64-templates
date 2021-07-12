#include "game.h"

#include <ultra64.h>
#include <nustd/math.h>

#include <os_host.h>

#include "definitions.h"
#include "math.h"
#include "static.h"
#include "controller.h"
#include "data/texture.h"
#include "fonts/font_ext.h"
#include "objects/billboards.h"
#include "objects/map_helper.h"
#include "objects/walls.h"

#include "../libs/ultra64-extensions/include/easing.h"
#include "../libs/ultra64-extensions/include/mem_pool.h"
#include "../libs/ultra64-extensions/include/tween.h"

#include "maps/maps.h"

// Task header
OSTask taskHeader = {
	M_GFXTASK,						 /* type of task */
	OS_TASK_DP_WAIT,				 /* flags - wait for DP to be available */
	NULL,							 /* ucode boot (fill in later) */
	0,								 /* ucode boot size (fill in later) */
	NULL,							 /* ucode (fill in later) */
	SP_UCODE_SIZE,					 /* ucode size */
	NULL,							 /* ucode data (fill in later) (to init DMEM) */
	SP_UCODE_DATA_SIZE,				 /* ucode data size */
	&dram_stack[0],					 /* stack used by ucode */
	SP_DRAM_STACK_SIZE8,			 /* size of stack */
	&rdp_output[0],					 /* output fifo buffer start */
	&rdp_output[0] + RDP_OUTPUT_LEN, /* output fifo buffer end */
	NULL,							 /* display list pointer (fill in later) */
	0,								 /* display list size (ignored) */
	NULL,							 /* yield buffer (used if yield will occur) */
	0								 /* yield buffer length */
};

// Viewport
Vp vp = {
	SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0, /* scale */
	SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0, /* translate */
};

// global variables
Gfx *glistp;		 /* RSP display list pointer */
Dynamic dynamic;	 /* dynamic data */
int draw_buffer = 0; /* frame buffer being updated (0 or 1) */
Map current_map;

// start font
int fontcol[4]; /* color for shadowed fonts */
#define FONT_COL 55, 155, 255, 255
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

#define MEM_POOL_SIZE (1024 * 1024)
char global_memory[MEM_POOL_SIZE];
MemZone memory_pool;

Tween *movement_tween;
Tween *view_tween;

// macros
#define AXISTHRESH 30
#define PADTHRESH(num)                                                                             \
	((num > AXISTHRESH) ? num - AXISTHRESH : (num < -AXISTHRESH) ? num + AXISTHRESH : 0)
#define IS_BUTTON_PRESSED(btn) (gd.pad[0]->button & btn)
#define MOVEMENT_SPEED 400

// This structure contains all position info
typedef struct {
	Vec3f pos;
	float view_speed;
	float move_forward;
	float move_lateral;
	Vec3f forward;
	float angle;
	u32 current_tile;
} PlayerData;
PlayerData pp;

// This structure contains movement and controller info
typedef struct {
	OSContPad **pad;
} GameData;
GameData gd;

typedef struct {
	OSTask *theadp;
	Dynamic *dynamicp;
	u16 perspnorm;
	Mat4 modmat;
	Mat4 m1, m2;
	Mat4 allmat;
} RenderData;
RenderData rd;

// helper functions for movement
void set_angle(float angle_diff);
void move_to(s32 h_speed, s32 forward_speed);

// tween callbacks
void movement_callback(void *target_object, Position current_value);
void view_callback(void *target_object, float current_value);

// called once to setup the game
void setup();

// called every frame to update data
void update();

// called every frame to render data to screen
void render_setup();   // setup viewport and matrixes
void render();		   // render the objects
void render_finish();  // send data to draw

// This is the main routine of the app.
void game(void) {
	setup();

	// Main game loop
	while (1) {
		update();

		// Render the game
		render_setup();
		render();
		render_finish();
	}
}

void setup() {
	mem_zone_init(&memory_pool, global_memory, MEM_POOL_SIZE);
	movement_tween = tween_init(&memory_pool);
	view_tween = tween_init(&memory_pool);

	current_map.data = map1_1;
	current_map.size = map1_1_size;
	current_map.width = map1_1_width;

	Vec3 player_start = map_get_start_position(&current_map, &pp.current_tile);
	pp.pos[0] = player_start.x;
	pp.pos[1] = player_start.y;
	pp.pos[2] = player_start.z;
	pp.angle = 0;

	set_angle(0);
	move_to(0, 0);
}

void update() {
	gd.pad = ReadController(0);

	tween_tick(movement_tween);
	tween_tick(view_tween);

	pp.move_forward = 0;
	pp.move_lateral = 0;
	pp.view_speed = 0;

	// move
	if (movement_tween->finished && view_tween->finished) {
		if (IS_BUTTON_PRESSED(U_JPAD) || PADTHRESH(gd.pad[0]->stick_y) > 0) {
			pp.move_forward = TILE_SIZE;
			move_to(pp.move_lateral, pp.move_forward);
		} else if (IS_BUTTON_PRESSED(D_JPAD) || PADTHRESH(gd.pad[0]->stick_y) < 0) {
			pp.move_forward = -TILE_SIZE;
			move_to(pp.move_lateral, pp.move_forward);
		} else if (IS_BUTTON_PRESSED(L_JPAD) || PADTHRESH(gd.pad[0]->stick_x) < 0) {
			pp.view_speed = -RAD_90;
			set_angle(pp.view_speed);
		} else if (IS_BUTTON_PRESSED(R_JPAD) || PADTHRESH(gd.pad[0]->stick_x) > 0) {
			pp.view_speed = RAD_90;
			set_angle(pp.view_speed);
		} else if (IS_BUTTON_PRESSED(L_TRIG) | IS_BUTTON_PRESSED(Z_TRIG)) {
			pp.move_lateral = -TILE_SIZE;
			move_to(pp.move_lateral, pp.move_forward);
		} else if (IS_BUTTON_PRESSED(R_TRIG)) {
			pp.move_lateral = TILE_SIZE;
			move_to(pp.move_lateral, pp.move_forward);
		}
	}
}

void render_setup() {
	// pointers to build the display list.
	rd.theadp = &taskHeader;
	rd.dynamicp = &dynamic;
	glistp = rd.dynamicp->glist;

	// Tell RCP where each segment is
	gSPSegment(glistp++, 0, 0x0); /* physical segment */
	gSPSegment(glistp++, STATIC_SEGMENT, OS_K0_TO_PHYSICAL(staticSegment));
	gSPSegment(glistp++, CFB_SEGMENT, OS_K0_TO_PHYSICAL(cfb[draw_buffer]));
	gSPSegment(glistp++, TEXTURE_SEGMENT, OS_K0_TO_PHYSICAL(textureSegment));

	// Clear z and color framebuffer.
	gSPDisplayList(glistp++, clearzbuffer_dl);
	gSPDisplayList(glistp++, clearcfb_dl);

	// Modify & specify Viewport
	gSPViewport(glistp++, &vp);

	// Initialize RCP state.
	gSPDisplayList(glistp++, init_dl);

	// set up matrices
	guPerspectiveF(rd.allmat, &rd.perspnorm, 80.0, 320.0 / 240.0, 1.0, 1024.0, 1.0);
	guPerspective(&(rd.dynamicp->projection), &rd.perspnorm, 80.0, 320.0 / 240.0, 1.0, 1024.0, 1.0);

	Vec3f forward = {pp.pos[0] + pp.forward[0], pp.pos[1] + 5.0, pp.pos[2] + pp.forward[2]};
	guLookAtF(rd.m2, pp.pos[0], forward[1], pp.pos[2], forward[0], forward[1], forward[2], 0.0, 1.0,
			  0.0);
	guLookAt(&(rd.dynamicp->viewing), pp.pos[0], forward[1], pp.pos[2], forward[0], forward[1],
			 forward[2], 0.0, 1.0, 0.0);

	guMtxCatF(rd.m2, rd.allmat, rd.m1);

	guScale(&(rd.dynamicp->modeling), 1.0, 1.0, 1.0);
	guScaleF(rd.modmat, 1.0, 1.0, 1.0);

	guMtxCatF(rd.modmat, rd.m1, rd.allmat);

	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(rd.dynamicp->projection)),
			  G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(rd.dynamicp->viewing)),
			  G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(rd.dynamicp->modeling)),
			  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

	gSPPerspNormalize(glistp++, rd.perspnorm);
	gSPClipRatio(glistp++, FRUSTRATIO_1);
}

void render() {
	map_render(&current_map, &glistp, rd.dynamicp);

	// render text
	font_init(&glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(200, 1);
	FONTCOLM(FONT_COL);
	char position[100];
	sprintf(position, "Tile: %d Dir: %.2f, %.2f", pp.current_tile, pp.forward[0], pp.forward[2]);
	SHOWFONT(&glistp, position, 20, 210);
	font_finish(&glistp);

	// billboard setup
	// gSPDisplayList(glistp++, billboard_texture_setup_dl);

	// plants
	// gSPTexture(glistp++, 1024 * 2, 1024 * 2, 0, G_TX_RENDERTILE, G_ON);
	// gDPLoadTextureBlock(glistp++, spr_plant, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
	// G_TX_WRAP, 					G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);

	// DRAW_PLANT(20, 20);
	// DRAW_PLANT(26, 20);
	// DRAW_PLANT(32, 20);

	// Finish rendering
	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
}

void render_finish() {
	// Build graphics task
	rd.theadp->t.ucode_boot = (u64 *)rspbootTextStart;
	rd.theadp->t.ucode_boot_size = (u32)rspbootTextEnd - (u32)rspbootTextStart;

	// RSP output over XBUS to RDP:
	rd.theadp->t.ucode = (u64 *)gspF3DEX2_xbusTextStart;
	rd.theadp->t.ucode_data = (u64 *)gspF3DEX2_xbusDataStart;

	// initial display list
	rd.theadp->t.data_ptr = (u64 *)rd.dynamicp->glist;
	rd.theadp->t.data_size = (u32)((glistp - rd.dynamicp->glist) * sizeof(Gfx));

	// Write back dirty cache lines that need to be read by the RCP.
	osWritebackDCache(&dynamic, sizeof(dynamic));

	// start up the RSP task
	osSpTaskStart(rd.theadp);

	// wait for DP completion
	(void)osRecvMesg(&rdpMessageQ, NULL, OS_MESG_BLOCK);

	// setup to swap buffers
	osViSwapBuffer(cfb[draw_buffer]);

	// Make sure there isn't an old retrace in queue (assumes queue has a depth of 1)
	if (MQ_IS_FULL(&retraceMessageQ))
		(void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);

	// Wait for Vertical retrace to finish swap buffers
	(void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
	draw_buffer ^= 1;
}

void set_angle(float angle_diff) {
	float final_angle = pp.angle + angle_diff;
	tween_restart(view_tween, &pp, &easing_exponential_out, MOVEMENT_SPEED, NULL, false, false);
	tween_set_to_float(view_tween, pp.angle, final_angle, &view_callback);
}

void move_to(s32 h_speed, s32 forward_speed) {
	bool path_is_blocked = false;

	Position final_position;
	if (forward_speed != 0) {
		s8 sign = forward_speed > 0 ? 1 : -1;
		s32 tile = pp.current_tile +
				   (((u32)pp.forward[0] + ((u32)pp.forward[2] * current_map.width)) * sign);

		path_is_blocked = map_is_tile_blocked(&current_map, tile);
		if (path_is_blocked) {
			final_position.x = pp.pos[0] + (pp.forward[0] * (forward_speed / 4));
			final_position.y = pp.pos[2] + (pp.forward[2] * (forward_speed / 4));
		} else {
			final_position.x = pp.pos[0] + (pp.forward[0] * forward_speed);
			final_position.y = pp.pos[2] + (pp.forward[2] * forward_speed);
			pp.current_tile = tile;
		}
	} else if (h_speed != 0) {
		s8 sign = h_speed > 0 ? 1 : -1;
		float x, y;
		get_forward_vector_from_angle(pp.angle + RAD_90, &x, &y);
		u32 tile = pp.current_tile + (((u32)x + ((u32)y * current_map.width)) * sign);

		path_is_blocked = map_is_tile_blocked(&current_map, tile);
		if (path_is_blocked) {
			final_position.x = pp.pos[0] + (x * (h_speed / 4));
			final_position.y = pp.pos[2] + (y * (h_speed / 4));
		} else {
			final_position.x = pp.pos[0] + (x * h_speed);
			final_position.y = pp.pos[2] + (y * h_speed);
			pp.current_tile = tile;
		}
	} else {
		final_position.x = pp.pos[0];
		final_position.y = pp.pos[2];
	}

	tween_restart(movement_tween, &pp, &easing_exponential_out, MOVEMENT_SPEED, NULL,
				  path_is_blocked, false);
	Position p = {pp.pos[0], pp.pos[2]};
	tween_set_to_position(movement_tween, p, final_position, &movement_callback);
}

void movement_callback(void *target_object, Position current_value) {
	pp.pos[0] = current_value.x;
	pp.pos[2] = current_value.y;
}

void view_callback(void *target_object, float current_value) {
	pp.angle = current_value;
	get_forward_vector_from_angle(pp.angle, &pp.forward[0], &pp.forward[2]);
}
