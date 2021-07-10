#pragma once

#define DRAW_WALL_X(x, y)                                                                          \
	gDPPipeSync(glistp++);                                                                         \
	guTranslate(&dynamic.object_position[obj_count], x, 0, y);                                     \
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.object_position[obj_count])),                  \
			  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);                                        \
	gSPDisplayList(glistp++, wall_dl);                                                             \
	obj_count++;

#define DRAW_WALL_Y(x, y)                                                                          \
	gDPPipeSync(glistp++);                                                                         \
	guTranslate(&dynamic.object_position[obj_count], x, 0, y);                                     \
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.object_position[obj_count])),                  \
			  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);                                        \
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.wall_y_rotation)),                             \
			  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);                                         \
	gSPDisplayList(glistp++, wall_dl);                                                             \
	obj_count++;
