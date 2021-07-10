#include "game.h"

#include <ultra64.h>
#include <nustd/math.h>

#include "definitions.h"
#include "math.h"
#include "static.h"
#include "controller.h"
#include "data/texture.h"
#include "objects/billboards.h"
#include "objects/walls.h"

#include "../libs/ultra64-extensions/include/easing.h"
#include "../libs/ultra64-extensions/include/mem_pool.h"
#include "../libs/ultra64-extensions/include/tween.h"

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
int obj_count;		 /* count of used objects on current frame */
int billboard_count; /* count of used billboards on current frame */

#define MEM_POOL_SIZE (1024 * 1024)
char global_memory[MEM_POOL_SIZE];
MemZone memory_pool;

Tween *movement_tween;
Tween *view_tween;

// macros
#define PADTHRESH(num, thresh) ((num > thresh) ? num - thresh : (num < -thresh) ? num + thresh : 0)
#define IS_BUTTON_PRESSED(btn) (gd.pad[0]->button & btn)

// This structure contains all position info
typedef struct {
	Vec3f pos;
	Vec3f eye;
	float view_speed;
	float move_forward;
	float move_lateral;
	Vec3f forward;
	float angle;
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
void move_to(float h_speed, float forward_speed);

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

	set_angle(0);
	move_to(0, 0);

	guRotate(&(dynamic.wall_y_rotation), 90, 0, 1, 0);

	pp.pos[0] = 0.0f;
	pp.pos[1] = 0.0f;
	pp.pos[2] = 0.0f;
	pp.eye[0] = 20.0f;
	pp.eye[1] = 20.0f;
	pp.eye[2] = 50.0f;
	pp.angle = 0;
}

void update() {
	obj_count = 0;
	billboard_count = 0;
	gd.pad = ReadController(U_JPAD | L_JPAD | R_JPAD | D_JPAD | L_TRIG | R_TRIG | Z_TRIG);

	tween_tick(movement_tween);
	tween_tick(view_tween);

	pp.move_forward = 0;
	pp.move_lateral = 0;
	pp.view_speed = 0;

	// move
	if (movement_tween->finished && view_tween->finished) {
		if (IS_BUTTON_PRESSED(U_JPAD)) {
			pp.move_forward = TILE_SIZE;
			move_to(pp.move_lateral, pp.move_forward);
		} else if (IS_BUTTON_PRESSED(D_JPAD)) {
			pp.move_forward = -TILE_SIZE;
			move_to(pp.move_lateral, pp.move_forward);
		} else if (IS_BUTTON_PRESSED(L_JPAD)) {
			pp.view_speed = -RAD_90;
			set_angle(pp.view_speed);
		} else if (IS_BUTTON_PRESSED(R_JPAD)) {
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
	guLookAtF(rd.m2, pp.pos[0], forward[1], pp.pos[2], pp.pos[0] + pp.forward[0], forward[1],
			  pp.pos[2] + pp.forward[2], 0.0, 1.0, 0.0);
	guLookAt(&(rd.dynamicp->viewing), pp.pos[0], forward[1], pp.pos[2], pp.pos[0] + pp.forward[0],
			 forward[1], pp.pos[2] + pp.forward[2], 0.0, 1.0, 0.0);

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
	// ground
	gSPDisplayList(glistp++, ground_texture_setup_dl);
	gSPTexture(glistp++, 65535, 65535, 0, G_TX_RENDERTILE, G_ON);
	gDPLoadTextureBlock(glistp++, spr_ground, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_WRAP,
						G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);

	gSPDisplayList(glistp++, ground_dl);

	// ceiling
	guTranslate(&dynamic.object_position[obj_count], 0, 10, 0);
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.object_position[obj_count])),
			  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
	gSPDisplayList(glistp++, ground_dl);
	obj_count++;

	// walls
	gSPTexture(glistp++, 1024 * 10, 1024 * 10, 0, G_TX_RENDERTILE, G_ON);
	gDPLoadTextureBlock(glistp++, spr_wall, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_WRAP,
						G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);

	DRAW_WALL_Y(5, 5);
	DRAW_WALL_Y(5, 15);
	DRAW_WALL_X(5, 15);
	DRAW_WALL_X(15, 15);
	DRAW_WALL_X(5, 5);
	DRAW_WALL_X(15, 5);

	// billboard setup
	gSPDisplayList(glistp++, billboard_texture_setup_dl);

	// plants
	gSPTexture(glistp++, 1024 * 2, 1024 * 2, 0, G_TX_RENDERTILE, G_ON);
	gDPLoadTextureBlock(glistp++, spr_plant, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_WRAP,
						G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);

	DRAW_PLANT(20, 20);
	DRAW_PLANT(26, 20);
	DRAW_PLANT(32, 20);

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
	tween_restart(view_tween, &pp, &easing_exponential_out, 200, NULL, false, false);
	tween_set_to_float(view_tween, pp.angle, final_angle, &view_callback);
}

void move_to(float h_speed, float forward_speed) {
	Position final_position;
	final_position.x = pp.pos[0] + pp.forward[0] * forward_speed;
	final_position.y = pp.pos[2] + pp.forward[2] * forward_speed;

	if (h_speed != 0) {
		float x, y;
		get_forward_vector_from_angle(pp.angle + RAD_90, &x, &y);
		final_position.x = pp.pos[0] + x * h_speed;
		final_position.y = pp.pos[2] + y * h_speed;
	}

	tween_start(movement_tween, &pp, &easing_exponential_out, 200, NULL, false, false);
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
