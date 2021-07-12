#include <ultra64.h>

static Vtx wall_east_vtx[] = {
	{10, 10, 10, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, -0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, -0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, 10, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};
static Vtx wall_south_vtx[] = {
	{10, -0, 10, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, 10, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, 10, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, 10, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};
static Vtx wall_west_vtx[] = {
	{-0, 10, -0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, 10, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, 10, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};
static Vtx wall_north_vtx[] = {
	{-0, -0, -0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, -0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, -0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};

static Vtx wall_square_vtx[] = {
	{-0, -0, -0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, -0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, -0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},

	{-0, 10, -0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, 10, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, 10, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},

	{10, 10, 10, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, -0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, -0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, 10, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},

	{10, -0, 10, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, 10, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, 10, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, 10, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};

Gfx wall_east_dl[] = {
	gsSPVertex(&(wall_east_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};
Gfx wall_south_dl[] = {
	gsSPVertex(&(wall_south_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};
Gfx wall_west_dl[] = {
	gsSPVertex(&(wall_west_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};
Gfx wall_north_dl[] = {
	gsSPVertex(&(wall_north_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};

Gfx wall_square_dl[] = {
	gsSPVertex(&(wall_square_vtx[0]), 16, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),			/**/
	gsSP2Triangles(4, 6, 5, 4, 6, 4, 7, 4),			/**/
	gsSP2Triangles(8, 10, 9, 8, 10, 8, 11, 8),		/**/
	gsSP2Triangles(12, 14, 13, 12, 14, 12, 15, 12), /**/
	gsSPEndDisplayList(),
};
