#pragma once

#define DRAW_BILLBOARD(x, y)                                                                       \
	{                                                                                              \
		float angle = (atan2f(y - pp.pos[2], x - pp.pos[0]) + RAD_90) * RAD_MULT;                  \
		guRotate(&(dynamic.billboard_rotation[billboard_count]), -angle, 0, 1, 0);                 \
		guTranslate(&(dynamic.object_position[obj_count]), x, 0, y);                               \
		gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.object_position[obj_count])),              \
				  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);                                    \
		gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.billboard_rotation[billboard_count])),     \
				  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);                                     \
		billboard_count++;                                                                         \
	}

#define DRAW_PLANT(x, y)                                                                           \
	gDPPipeSync(glistp++);                                                                         \
	DRAW_BILLBOARD(x, y);                                                                          \
	gSPDisplayList(glistp++, plant_dl);                                                            \
	obj_count++;
