#include "boot.h"

#include "types.h"

/*
 * Message queues
 */
extern OSMesgQueue rdpMessageQ, retraceMessageQ;

/*
 * global variables
 */
extern int rdp_flag;
extern char *staticSegment;
extern char *textureSegment;
extern char *position_str;

#define GLIST_LEN 2048

/* buffer size for RDP DL */
#define RDP_OUTPUT_LEN (4096 * 16)

/* used for matrix stack */
extern u64 dram_stack[];

/* buffer for RDP DL */
extern u64 rdp_output[];

#define MAX_OBJECTS 100
#define MAX_BILLBOARDS 100
/*
 * Layout of dynamic data.
 *
 * This structure holds the things which change per frame. It is advantageous
 * to keep dynamic data together so that we may selectively write back dirty
 * data cache lines to DRAM prior to processing by the RCP.
 *
 */
typedef struct {
	Mtx projection;
	Mtx modeling;
	Mtx ballmodel;
	Mtx ballshadowmodel;
	Mtx ballshadowmodel2;
	Mtx viewing;
	Mtx wall_y_rotation;
	Mtx billboard_rotation[MAX_BILLBOARDS];
	Mtx object_position[MAX_OBJECTS];
	Gfx glist[GLIST_LEN];
} Dynamic;

extern Dynamic dynamic;

/*
 * frame buffer symbols
 */
extern u16 zbuffer[];					 /* RAM address */
extern u16 cfb[][SCREEN_WD * SCREEN_HT]; /* RAM address */
extern u16 rsp_cfb[];					 /* segment address */
