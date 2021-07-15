#include <ultra64.h>
#include "game.h"

/*
 * buffers for RSP tasks:
 */

/*
 * buffers used by fifo microcode only
 */

/*  for CodeWarrior compiler */
#ifdef __MWERKS__
#pragma align(16)
#endif

#if __GNUC__ /* { */

/* for GNU compiler */
u64 dram_stack[SP_DRAM_STACK_SIZE64] __attribute__((aligned(16))); /* used for matrix stack */
u64 rdp_output[RDP_OUTPUT_LEN] __attribute__((aligned(16)));	   /* buffer for RDP DL */

#else /* }{ */

/* for SGI compiler */
u64 dram_stack[SP_DRAM_STACK_SIZE64]; /* used for matrix stack */
u64 rdp_output[RDP_OUTPUT_LEN];		  /* buffer for RDP DL */

#endif /* } */
