#include "graphic.h"

#include "definitions.h"

Gfx glist[GLIST_LENGTH];
Gfx *glistp;

static Vp viewport = {
	SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0, SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0,
};

Gfx rspinit_dl[] = {
	gsSPViewport(&viewport),
	gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH | G_FOG | G_LIGHTING |
						  G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD),
	gsSPTexture(0, 0, 0, 0, G_OFF),
	gsSPEndDisplayList(),
};

Gfx rdpinit_dl[] = {
	gsDPSetCycleType(G_CYC_1CYCLE),
	gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),
	gsDPSetCombineKey(G_CK_NONE),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsDPSetRenderMode(G_RM_NOOP, G_RM_NOOP2),
	gsDPSetColorDither(G_CD_DISABLE),
	gsDPPipeSync(),
	gsSPEndDisplayList(),
};

void rcp_init(Gfx *dlist) {
	gSPSegment(dlist++, 0, 0);
	gSPDisplayList(dlist++, OS_K0_TO_PHYSICAL(rspinit_dl));
	gSPDisplayList(dlist++, OS_K0_TO_PHYSICAL(rdpinit_dl));
}
