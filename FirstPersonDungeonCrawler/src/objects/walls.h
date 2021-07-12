#pragma once

#define DRAW_WALL_SQUARE(x, y)                                                                     \
	gDPPipeSync((*glistp)++);                                                                      \
	guTranslate(&dynamicp->object_position[obj_count], x, 0, y);                                   \
	gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamicp->object_position[obj_count])),             \
			  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);                                        \
	gSPDisplayList((*glistp)++, wall_square_dl);                                                   \
	obj_count++;

#define DRAW_WALL_EAST(x, y)                                                                       \
	gDPPipeSync((*glistp)++);                                                                      \
	guTranslate(&dynamicp->object_position[obj_count], x, 0, y);                                   \
	gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamicp->object_position[obj_count])),             \
			  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);                                        \
	gSPDisplayList((*glistp)++, wall_east_dl);                                                     \
	obj_count++;

#define DRAW_WALL_SOUTH(x, y)                                                                      \
	gDPPipeSync((*glistp)++);                                                                      \
	guTranslate(&dynamicp->object_position[obj_count], x, 0, y);                                   \
	gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamicp->object_position[obj_count])),             \
			  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);                                        \
	gSPDisplayList((*glistp)++, wall_south_dl);                                                    \
	obj_count++;

#define DRAW_WALL_WEST(x, y)                                                                       \
	gDPPipeSync((*glistp)++);                                                                      \
	guTranslate(&dynamicp->object_position[obj_count], x, 0, y);                                   \
	gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamicp->object_position[obj_count])),             \
			  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);                                        \
	gSPDisplayList((*glistp)++, wall_west_dl);                                                     \
	obj_count++;

#define DRAW_WALL_NORTH(x, y)                                                                      \
	gDPPipeSync((*glistp)++);                                                                      \
	guTranslate(&dynamicp->object_position[obj_count], x, 0, y);                                   \
	gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamicp->object_position[obj_count])),             \
			  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);                                        \
	gSPDisplayList((*glistp)++, wall_north_dl);                                                    \
	obj_count++;
