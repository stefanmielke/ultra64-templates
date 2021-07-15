#pragma once

#define DRAW_GUN(x, y)                                                                             \
	{                                                                                              \
		float pos_x = x + pp.forward[0] * 10;                                                      \
		float pos_y = y + pp.forward[2] * 10;                                                      \
		float angle = (atan2f(pos_y - pp.pos[2], pos_x - pp.pos[0]) + (RAD_90 / 6)) * RAD_MULT;    \
		guRotate(&(dynamic.billboard_rotation[billboard_count]), -angle, 0, 1, 0);                 \
		guTranslate(&(dynamic.object_position[obj_count]), pos_x, 1, pos_y);                       \
		gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.object_position[obj_count])),              \
				  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);                                    \
		gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.billboard_rotation[billboard_count])),     \
				  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);                                     \
		billboard_count++;                                                                         \
		obj_count++;                                                                               \
	}

#define DRAW_GUN_THEGUN()                                                                          \
	gDPPipeSync(glistp++);                                                                         \
	DRAW_GUN(pp.pos[0], pp.pos[2]);                                                                \
	gSPDisplayList(glistp++, gun_dl);
