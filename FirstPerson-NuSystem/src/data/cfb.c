/*
 * RSP view of the frame buffer.  It exists to create an RSP address for
 * the frame buffer, which is remapped on each frame to either of two
 * regions of physical memory that store that actual bits.
 */

#include <ultra64.h>
#include "boot.h"

u16 cfb[2][SCREEN_WD * SCREEN_HT];
