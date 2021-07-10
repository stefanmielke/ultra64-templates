#include <ultra64.h>

static Vtx wall_vtx[] = {
	{-0, 10, 0, 0, 00 << 10, 00 << 10, 0xff, 0x00, 0x00, 0xff},
	{10, 10, 0, 0, 32 << 10, 00 << 10, 0x00, 0xff, 0x00, 0xff},
	{10, -0, -0, 0, 32 << 10, 32 << 10, 0x00, 0x00, 0xff, 0xff},
	{-0, -0, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};

Gfx wall_dl[] = {
	gsSPVertex(&(wall_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};
