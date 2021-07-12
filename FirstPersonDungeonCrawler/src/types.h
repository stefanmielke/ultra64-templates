#pragma once

#include <ultra64.h>

typedef f32 Vec3f[3];  // X, Y, Z, where Y is up
typedef f32 Mat4[4][4];

// X, Y, Z, where Y is up
typedef struct {
	u32 x;
	u32 y;
	u32 z;
} Vec3;