#pragma once

#include <ultra64.h>

typedef struct {
	u32 x;
	u32 y;
} Player;

void player_init(Player *player, u32 start_x, u32 start_y);
void player_move(Player *player, u32 x_add, u32 y_add);
void player_teleport(Player *player, u32 new_x, u32 new_y);
