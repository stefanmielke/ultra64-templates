#include <ultra64.h>

static Vtx gun_vtx[] = {
	{9, 3, 0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{0, 3, 3, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{0, 0, 3, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{9, 0, 0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
};

Gfx gun_dl[] = {
	gsSPVertex(&(gun_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};

Gfx gun_texture_setup_dl[] = {
	gsDPPipeSync(),

	gsDPSetCycleType(G_CYC_1CYCLE),
	gsSPClearGeometryMode(G_SHADING_SMOOTH | G_ZBUFFER),
	gsSPSetGeometryMode(G_SHADE | G_CULL_BACK),
	gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2),
	gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),

	gsDPSetTexturePersp(G_TP_PERSP),

	gsSPEndDisplayList(),
};
