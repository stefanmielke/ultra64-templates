#include "graphic.h"

#include "definitions.h"

Gfx glist[GLIST_LENGTH];
Gfx *glistp;

void rcp_init(Gfx *dlist) {
	gSPSegment(dlist++, 0, 0);
}
