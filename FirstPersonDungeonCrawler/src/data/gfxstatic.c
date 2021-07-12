/*
 * File:	gfxstatic.c
 *
 * This file holds display list segments that are 'static' data.
 *
 */

#include <ultra64.h>
#include "../boot.h"
#include "../game.h"
#include "texture.h"

static Vtx ground_vtx[] = {
	{00, 0, 00, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 0, 00, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 0, 10, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{00, 0, 10, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};

static Vtx ceiling_vtx[] = {
	{00, 0, 10, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 0, 10, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 0, 00, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{00, 0, 00, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};

Gfx ground_texture_setup_dl[] = {
	gsDPPipeSync(),

	gsDPSetCycleType(G_CYC_1CYCLE),
	gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_ZBUFFER | G_CULL_BACK),
	gsDPSetRenderMode(G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2),
	gsDPSetCombineMode(G_CC_BLENDRGBA, G_CC_BLENDRGBA),
	gsDPSetTexturePersp(G_TP_PERSP),

	gsSPEndDisplayList(),
};

Gfx ground_dl[] = {
	gsSPVertex(&(ground_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};

Gfx ceiling_dl[] = {
	gsSPVertex(&(ceiling_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};
