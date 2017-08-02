#include "guithread.h"
#include "font_ttf.h"
#include <sf2d.h>
void guithread(void * arg)
{	
	sf2d_init();
	sf2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));
	sftd_init();
	font = sftd_load_font_mem(font_ttf,font_ttf_size);
	while(runThread)
	{	
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
		gui.topFunc();
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		gui.botFunc();
		sf2d_swapbuffers();
	}
	sftd_free_font(font);
	sftd_fini();
	sf2d_fini();
	threadExit(0);
}