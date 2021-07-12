
extern unsigned short map1_1[];
extern unsigned long map1_1_size;
extern unsigned long map1_1_width;

typedef enum {
	TT_Empty_Space = 0,
	// walls
	TT_Wall_Full = 1,
	TT_Wall_East = 2,
	TT_Wall_South = 3,
	TT_Wall_West = 4,
	TT_Wall_North = 5,

	// grounds
	TT_Ground = 100,

	// objects
	TT_StartPos = 255,
} TileTypes;

typedef enum {
	TL_Empty_Space = 0,

	TL_Wall_Start = 1,
	TL_Wall_End = 10,

	TL_Ground_Start = 100,
	TL_Ground_End = 254,

	TL_Objects_Start = 255,
	TL_Objects_End = 65535,
} TileLimits;