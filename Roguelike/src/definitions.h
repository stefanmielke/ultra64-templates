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
#define ButtonWasPressed(player_id, button) (contdata[(player_id)].trigger & (button))
#define ButtonIsHeld(player_id, but) (contdata[(player_id)].button & (but))
