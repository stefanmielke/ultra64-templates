#include <nusys.h>

#include "../graphic.h"
#include "../definitions.h"

#include "../data/spr_pyoro_walk1.h"

u16 pyoro_pos_x, pyoro_pos_y;
float pyoro_scale_x, pyoro_scale_y, pyoro_rot;
u8 b;
float scale_inc;

void clear_background(u8 r, u8 g, u8 b);
void draw_rect(u16 x, u16 y, u16 w, u16 h, u8 r, u8 g, u8 b);
void draw_pyoro(u16 x, u16 y);
void draw_pyoro_scaled(int x, int y, float scale_x, float scale_y);
void draw_pyoro_rotated(int x, int y, double ang);

void stage00_init(void) {
	b = 255;
	pyoro_pos_x = 32;
	pyoro_pos_y = 32;
	pyoro_rot = 0;
	pyoro_scale_x = 1;
	pyoro_scale_y = 1;
	scale_inc = 0.01f;
}

void stage00_update(void) {
	b -= 1;
	pyoro_rot += 1;
	pyoro_scale_x += scale_inc;
	pyoro_scale_y += scale_inc;

	if (pyoro_scale_x >= 2 || pyoro_scale_x < 0.5f)
		scale_inc *= -1;
}

void stage00_draw(void) {
	glistp = glist;
	rcp_init(glistp);

	clear_background(21, 22, 29);

	// this can be set _per sprite_ if needed
	gDPSetCycleType(glistp++, G_CYC_1CYCLE);
	gDPSetCombineMode(glistp++, G_CC_DECALRGBA, G_CC_DECALRGBA);
	gDPSetRenderMode(glistp++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE);
	gDPSetDepthImage(glistp++, nuGfxZBuffer);
	gDPSetDepthSource(glistp++, G_ZS_PRIM);
	gDPSetPrimDepth(glistp++, 0, 0);
	gDPSetTexturePersp(glistp++, G_TP_NONE);

	draw_pyoro(pyoro_pos_x, pyoro_pos_y);
	draw_pyoro_scaled(pyoro_pos_x + 32, pyoro_pos_y, pyoro_scale_x, pyoro_scale_y);
	draw_pyoro_rotated(pyoro_pos_x + 64, pyoro_pos_y, pyoro_rot);

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
	nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx), NU_GFX_UCODE_F3DEX2,
				   NU_SC_SWAPBUFFER);

	// renders console output
	// nuDebConDisp(NU_SC_NOSWAPBUFFER);
	// nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx), NU_GFX_UCODE_F3DEX2,
	// 			   NU_SC_SWAPBUFFER);
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

void draw_rect(u16 x, u16 y, u16 w, u16 h, u8 r, u8 g, u8 b) {
	gDPSetCycleType(glistp++, G_CYC_FILL);
	gDPSetDepthImage(glistp++, nuGfxZBuffer);
	gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
					 osVirtualToPhysical(nuGfxCfb_ptr));
	gDPSetFillColor(glistp++, (GPACK_RGBA5551(r, g, b, 1) << 16 | GPACK_RGBA5551(r, g, b, 1)));
	gDPFillRectangle(glistp++, x, y, x + w, y + h);
	gDPPipeSync(glistp++);
}

void draw_pyoro(u16 x, u16 y) {
	gDPLoadTextureBlock(glistp++, spr_pyoro_walk1, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0,
						G_TX_MIRROR, G_TX_CLAMP, 4, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
	gSPTextureRectangle(glistp++, (int)(x - 8) << 2, (int)(y - 8) << 2, (int)(x + 8) << 2,
						(int)(y + 8) << 2, G_TX_RENDERTILE, (0) << 5, 0 << 5, 1 << 10, 1 << 10);
	gDPPipeSync(glistp++);
}

void draw_pyoro_scaled(int x, int y, float scale_x, float scale_y) {
	float w = 16 * scale_x;
	float h = 16 * scale_y;
	s32 sx = (int)((1 << 10) / scale_x + 0.5F);
	s32 sy = (int)((1 << 10) / scale_y + 0.5F);

	gDPLoadTextureBlock(glistp++, spr_pyoro_walk1, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0,
						G_TX_CLAMP, G_TX_CLAMP, 4, 4,  // 2 to the power of 4 is 16
						G_TX_NOLOD, G_TX_NOLOD);
	gSPTextureRectangle(glistp++, ((int)(x - w / 2)) << 2, ((int)(y - h / 2)) << 2,
						((int)(x + w / 2)) << 2, ((int)(y + h / 2)) << 2, G_TX_RENDERTILE, 0 << 5,
						0 << 5, sx, sy);

	gDPPipeSync(glistp++);
}

void draw_pyoro_rotated(int x, int y, double ang) {
	int i, j;
	float rad = ang * 0.017453;	 // 1 degree ~= 0.017453 radians
	float co = cosf(rad);
	float si = sinf(rad);
	u16 *image = spr_pyoro_walk1;

	u16 newimage[16 * 16];
	// Wipe with a blank color
	for (i = 0; i < 16 * 16; i++)
		newimage[i] = 0;

	// Create the rotated sprite array
	for (i = 0; i < 16; i++)
		for (j = 0; j < 16; j++) {
			int x = (float)(i - 8) * co - (float)(j - 8) * si + 8;
			int y = (float)(i - 8) * si + (float)(j - 8) * co + 8;
			if (x < 0 || x >= 16 || y < 0 || y >= 16)
				continue;  // Ignore points outside our range
			newimage[i + j * 16] = image[y * 16 + x];
		}

	gDPLoadTextureBlock(glistp++, newimage, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0, G_TX_CLAMP,
						G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
	gSPTextureRectangle(glistp++, x - 8 << 2, y - 8 << 2, x + 8 << 2, y + 8 << 2, G_TX_RENDERTILE,
						0 << 5, 0 << 5, 1 << 10, 1 << 10);

	gDPPipeSync(glistp++);
}
