#pragma once

#define load_spr_peons(dlist)                                                                      \
	gDPLoadTextureBlock(dlist++, spr_peons, G_IM_FMT_RGBA, G_IM_SIZ_16b, 12, 12, 0, G_TX_CLAMP,    \
						G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

#define SPR_PEON_SIZE 6
#define SPR_PEON_SIZE_HALF 3

#define draw_spr_peon(dlist, x, y, number)                                                         \
	{                                                                                              \
		int __s_x = SPR_PEON_SIZE * ((number) % 2);                                                \
		int __s_y = SPR_PEON_SIZE * ((number) / 2);                                                \
		gSPTextureRectangle(                                                                       \
			dlist++, (int)((x)-SPR_PEON_SIZE_HALF) << 2, (int)((y)-SPR_PEON_SIZE_HALF) << 2,       \
			(int)((x) + SPR_PEON_SIZE_HALF) << 2, (int)((y) + SPR_PEON_SIZE_HALF) << 2,            \
			G_TX_RENDERTILE, __s_x << 5, __s_y << 5, 1 << 10, 1 << 10);                            \
	}

extern unsigned short spr_peons[];
