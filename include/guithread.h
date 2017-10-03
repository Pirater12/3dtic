#include <3ds.h>
#include <stdbool.h>

bool runThread;

typedef void (*guiFunc)(void);
typedef struct
{
	guiFunc botFunc;
	guiFunc topFunc;
}Gui;
Gui gui;
void guithread(void * arg);