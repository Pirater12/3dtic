#include <3ds.h>
#include <stdbool.h>
#include <sftd.h>
bool runThread;
sftd_font *font;
typedef void (*guiFunc)(void);
typedef struct
{
	guiFunc botFunc;
	guiFunc topFunc;
}Gui;
Gui gui;
void guithread(void * arg);