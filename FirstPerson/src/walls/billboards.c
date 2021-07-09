#include <ultra64.h>

#define PLANT_SIZE 5
static Vtx plant_vtx[] = {
	{-0, PLANT_SIZE, 0, 0, 00 << 10, 32 << 10, 0xff, 0x00, 0x00, 0xff},
	{PLANT_SIZE, PLANT_SIZE, 0, 0, 32 << 10, 32 << 10, 0x00, 0xff, 0x00, 0xff},
	{PLANT_SIZE, -0, -0, 0, 32 << 10, 00 << 10, 0x00, 0x00, 0xff, 0xff},
	{-0, -0, -0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
};

Gfx plant_dl[] = {
	gsSPVertex(&(plant_vtx[0]), 4, 0),
	gsSP1Triangle(0, 2, 1, 0),
	gsSP1Triangle(2, 0, 3, 0),
	gsSPEndDisplayList(),
};

Gfx billboard_texture_setup_dl[] = {
	gsDPPipeSync(),

	gsDPSetCycleType(G_CYC_1CYCLE),
	gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH),
	gsSPSetGeometryMode(G_ZBUFFER),
	gsDPSetRenderMode(G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2),
	gsDPSetCombineMode(G_CC_BLENDRGBDECALA, G_CC_BLENDRGBDECALA),

	gsDPSetTexturePersp(G_TP_PERSP),

	gsSPEndDisplayList(),
};