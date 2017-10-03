#include "../pp2d/pp2d/pp2d.h"
#include "guithread.h"
void guithread(void * arg)
{	
	pp2d_init();
	pp2d_set_screen_color(GFX_TOP, ABGR8(0xFF, 0x40, 0x40, 0x40));
	pp2d_set_screen_color(GFX_BOTTOM, ABGR8(0xFF, 0x40, 0x40, 0x40));
	while(runThread)
	{	
		pp2d_begin_draw(GFX_TOP);
		gui.topFunc();
		pp2d_draw_on(GFX_BOTTOM);
		gui.botFunc();
		pp2d_end_draw();
	}
	pp2d_exit();
	threadExit(0);
}