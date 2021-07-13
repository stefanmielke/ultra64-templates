#pragma once

#include <nusys.h>

#define SCREEN_WD 320
#define SCREEN_HT 240

#define GLIST_LENGTH 2048

extern Gfx glist[];
extern Gfx *glistp;

extern NUContData contdata[];
extern u8 contPattern;

#define CheckController(cont) (contPattern & (1 << (cont)))
