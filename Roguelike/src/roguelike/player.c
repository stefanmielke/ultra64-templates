#include "player.h"

void player_init(Player *player, u32 start_x, u32 start_y) {
	player->x = start_x;
	player->y = start_y;
}
void player_move(Player *player, u32 x_add, u32 y_add) {
	player->x += x_add;
	player->y += y_add;
}
void player_teleport(Player *player, u32 new_x, u32 new_y) {
	player->x = new_x;
	player->y = new_y;
}
