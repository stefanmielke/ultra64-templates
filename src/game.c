#include <ultra64.h>

#include "game.h"
#include "static.h"
#include "controller.h"
#include "data/texture.h"

/*
 * Task header.
 */
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

/*
 * Viewport
 */
Vp vp = {
	SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0, /* scale */
	SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0, /* translate */
};

/*
 * global variables
 */
Gfx *glistp;		 /* RSP display list pointer */
Dynamic dynamic;	 /* dynamic data */
int draw_buffer = 0; /* frame buffer being updated (0 or 1) */

/*
 * macros
 */
#define PADTHRESH(num, thresh) ((num > thresh) ? num - thresh : (num < -thresh) ? num + thresh : 0)
#define ABS(x) ((x > 0) ? x : -x)
#define PI (3.14159)

int wall_count;

#define DRAW_WALL_X(x, y)                                                                          \
	guTranslate(&dynamic.wall_position[wall_count], x, y, 0);                                      \
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.wall_position[wall_count])),                   \
			  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);                                        \
	gSPDisplayList(glistp++, wall_x_dl);                                                           \
	wall_count++;

#define DRAW_WALL_Y(x, y)                                                                          \
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.wall_y_rotation)),                             \
			  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);                                        \
	guTranslate(&dynamic.wall_position[wall_count], x, 0, y);                                      \
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.wall_position[wall_count])),                   \
			  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);                                         \
	gSPDisplayList(glistp++, wall_x_dl);                                                           \
	wall_count++;

/*
 * This structure is contains all position and menu info
 */
typedef struct Position {
	float pos[3];
	float eye[3];
	float forward[3];
	float angle;
} Position;
Position pp;

void get_forward_vector_from_angle(float angle, float *x, float *y) {
	*x = cosf(angle);
	*y = sinf(angle);
}

void set_angle(float angle_diff) {
	pp.angle += angle_diff;
	get_forward_vector_from_angle(pp.angle, &pp.forward[0], &pp.forward[2]);
}

void move_to(float h_speed, float forward_speed) {
	pp.pos[0] += pp.forward[0] * forward_speed;
	pp.pos[2] += pp.forward[2] * forward_speed;

	if (h_speed != 0) {
		float x, y;
		get_forward_vector_from_angle(pp.angle + 1.570796, &x, &y);
		pp.pos[0] += x * h_speed;
		pp.pos[2] += y * h_speed;
	}
}

/*
 * This is the main routine of the app.
 */
void game(void) {
	OSTask *theadp;
	Dynamic *dynamicp;
	OSContPad **pad;
	u16 perspnorm;
	float view_speed, move_forward, move_lateral;
	float modmat[4][4];
	float m1[4][4];
	float m2[4][4];
	float allmat[4][4];
	float balla = -0.02, ballv = 0.0, ballp = 0.0, ballvi = 0.3;

	guRotate(&(dynamic.wall_y_rotation), 90, 0, 1, 0);

	pp.pos[0] = 0.0f;
	pp.pos[1] = 0.0f;
	pp.pos[2] = 0.0f;
	pp.eye[0] = 20.0f;
	pp.eye[1] = 20.0f;
	pp.eye[2] = 50.0f;
	pp.angle = 0;

	/*
	 * Main game loop
	 */
	while (1) {
		wall_count = 0;
		pad = ReadController(0);

		view_speed = PADTHRESH(pad[0]->stick_x, 10);
		move_forward = PADTHRESH(pad[0]->stick_y, 10);
		view_speed *= ABS(view_speed);
		move_forward *= ABS(move_forward);

		move_lateral = pad[0]->button & L_CBUTTONS ? -1.0 : pad[0]->button & R_CBUTTONS ? 1.0 : 0;

		set_angle(view_speed / 100000.0);

		move_to(move_lateral, move_forward / 5000.0);

		/*
		 * pointers to build the display list.
		 */
		theadp = &taskHeader;
		dynamicp = &dynamic;
		glistp = dynamicp->glist;

		/*
		 * Tell RCP where each segment is
		 */
		gSPSegment(glistp++, 0, 0x0); /* physical segment */
		gSPSegment(glistp++, STATIC_SEGMENT, OS_K0_TO_PHYSICAL(staticSegment));
		gSPSegment(glistp++, CFB_SEGMENT, OS_K0_TO_PHYSICAL(cfb[draw_buffer]));
		gSPSegment(glistp++, TEXTURE_SEGMENT, OS_K0_TO_PHYSICAL(textureSegment));

		/*
		 * Clear z and color framebuffer.
		 */
		gSPDisplayList(glistp++, clearzbuffer_dl);
		gSPDisplayList(glistp++, clearcfb_dl);

		/*
		 * Modify & specify Viewport
		 */
		vp.vp.vscale[0] = SCREEN_WD * 2;
		vp.vp.vscale[1] = SCREEN_HT * 2;
		gSPViewport(glistp++, &vp);

		/*
		 * Initialize RCP state.
		 */
		gSPDisplayList(glistp++, init_dl);

		/*
		 * set up matrices
		 */
		guPerspectiveF(allmat, &perspnorm, 30.0, 320.0 / 240.0, 1.0, 1024.0, 1.0);
		guPerspective(&(dynamicp->projection), &perspnorm, 30.0, 320.0 / 240.0, 1.0, 1024.0, 1.0);

		guLookAtF(m2, pp.eye[0], pp.pos[1] + 5.0, pp.pos[2], pp.pos[0] + pp.forward[0],
				  pp.pos[1] + 5.0, pp.pos[2] + pp.forward[2], 0.0, 1.0, 0.0);
		guLookAt(&(dynamicp->viewing), pp.pos[0], pp.pos[1] + 5.0, pp.pos[2],
				 pp.pos[0] + pp.forward[0], pp.pos[1] + 5.0, pp.pos[2] + pp.forward[2], 0.0, 1.0,
				 0.0);

		guMtxCatF(m2, allmat, m1);

		guScale(&(dynamicp->modeling), 1.0, 1.0, 1.0);
		guScaleF(modmat, 1.0, 1.0, 1.0);

		guMtxCatF(modmat, m1, allmat);

		gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->projection)),
				  G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
		gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->viewing)),
				  G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
		gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->modeling)),
				  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

		gSPPerspNormalize(glistp++, perspnorm);

		gSPClipRatio(glistp++, FRUSTRATIO_1);

		/*
		 *  render triangles
		 */
		gSPDisplayList(glistp++, ground_texture_setup_dl);

		gSPTexture(glistp++, 65535.0, 65535.0, 0, G_TX_RENDERTILE, G_ON);
		gDPLoadTextureBlock(glistp++, RGBA16molecule, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
							G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);

		gSPDisplayList(glistp++, ground_dl);
		gSP1Triangle(glistp++, 0, 2, 1, 0);
		gSP1Triangle(glistp++, 2, 0, 3, 0);

		DRAW_WALL_Y(0, 0);
		DRAW_WALL_Y(0, 10);
		DRAW_WALL_X(0, 0);
		DRAW_WALL_X(10, 0);

		gDPFullSync(glistp++);
		gSPEndDisplayList(glistp++);

		/*
		 * Build graphics task:
		 *
		 */
		theadp->t.ucode_boot = (u64 *)rspbootTextStart;
		theadp->t.ucode_boot_size = (u32)rspbootTextEnd - (u32)rspbootTextStart;

		/*
		 * RSP output over XBUS to RDP:
		 */
		theadp->t.ucode = (u64 *)gspF3DEX2_xbusTextStart;
		theadp->t.ucode_data = (u64 *)gspF3DEX2_xbusDataStart;

		/*
		 * initial display list:
		 */
		theadp->t.data_ptr = (u64 *)dynamicp->glist;
		theadp->t.data_size = (u32)((glistp - dynamicp->glist) * sizeof(Gfx));

		/*
		 * Write back dirty cache lines that need to be read by the RCP.
		 */
		osWritebackDCache(&dynamic, sizeof(dynamic));

		/*
		 * start up the RSP task
		 */
		osSpTaskStart(theadp);

		/*
		 * wait for DP completion
		 */
		(void)osRecvMesg(&rdpMessageQ, NULL, OS_MESG_BLOCK);

		/*
		 * setup to swap buffers
		 */
		osViSwapBuffer(cfb[draw_buffer]);

		/*
		 * Make sure there isn't an old retrace in queue
		 * (assumes queue has a depth of 1)
		 */
		if (MQ_IS_FULL(&retraceMessageQ))
			(void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);

		/*
		 * Wait for Vertical retrace to finish swap buffers
		 */
		(void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
		draw_buffer ^= 1;
	}
}
