#include <ultra64.h>

static Vtx wall_x_vtx[] = {
	{-0, 10, 0, 0, 00 << 10, 00 << 10, 0xff, 0x00, 0x00, 0xff},
	{10, 10, 0, 0, 32 << 10, 00 << 10, 0x00, 0xff, 0x00, 0xff},
	{10, -0, -0, 0, 32 << 10, 32 << 10, 0x00, 0x00, 0xff, 0xff},
	{-0, -0, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};

Gfx wall_x_dl[] = {
	gsSPVertex(&(wall_x_vtx[0]), 4, 0),
	gsSP1Triangle(0, 2, 1, 0),
	gsSP1Triangle(2, 0, 3, 0),
	gsSPEndDisplayList(),
};
