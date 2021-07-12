#pragma once

#include <ultra64.h>

#define TILE_SIZE 10

typedef struct {
	u16 *data;
	u32 size;
	u32 width;
} Map;
