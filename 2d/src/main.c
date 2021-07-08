#include <nusys.h>

#include "definitions.h"
#include "screens/stage00.h"

/* Declaration of the prototype */
void vsyncCallback(int);

NUContData contdata[MAXCONTROLLERS];  // Number of controllers to keep track of (1)
u8 contPattern;						  // Which controllers are plugged in

/*------------------------
	Main
--------------------------*/
void mainproc(void) {
	/* The initialization of graphic  */
	nuGfxInit();

	contPattern = nuContInit();

	osViSetMode(&osViModeTable[OS_VI_NTSC_LPN1]);
	nuGfxFuncSet((NUGfxFunc)vsyncCallback);
	nuGfxDisplayOn();

	stage00_init();

	while (1)
		;
}

/*-----------------------------------------------------------------------------
  The call-back function

  pendingGfx which is passed from Nusystem as the argument of the call-back
  function is the total number of RCP tasks that are currently processing
  and waiting for the process.
-----------------------------------------------------------------------------*/
void vsyncCallback(int pendingGfx) {
	nuContDataGetExAll(contdata);

	stage00_update();
	/* It provides the display process if there is no RCP task that is processing. */
	if (pendingGfx < 1) {
		stage00_draw();
	}
}
