#include <3ds.h>
#include "../pp2d/pp2d/pp2d.h"
#include <stdbool.h>
#include <stdlib.h>
#include "guithread.h"
#include "table_png.h"
#include "paper_png.h"
typedef struct 
{
	char player1opt;
	char player2opt;
	int chance;
}Game;

Game game;

char *username = NULL;
char board[10]={' ',' ',' ',' ',' ',' ',' ',' ',' '};
int arr[9][2] = {{60,-15},{140,-15},{220,-15},{60,65},{140,65},{220,65},{60,145},{140,145},{220,145}};

void pp2d_draw_textf_center(gfxScreen_t target, float y, float scaleX, float scaleY, u32 color, const char* format, ...)
{
	va_list arg;
	char buf[256];
	va_start (arg, format);
	vsprintf(buf, format, arg);
	va_end (arg);
	pp2d_draw_text_center(target, y, scaleX, scaleY, color, buf);
}
bool checkWin(int x, char option)
{
	for(int i = 0; i<=8; i++){
	/*Check Vertical*/
	if(board[i] == option && board[i+3] == option && board[i+6] == option)
		return true;
	/*Check Horizontal*/
	else if((board[0] == option && board[1] == option && board[2] == option)||(board[3] == option && board[4] == option && board[5] == option)||(board[6] == option && board[7] == option && board[8] == option))
		return true;
}
	/*Check Diagnol*/
	if((board[2] == option && board[4] == option && board[6] == option) || (board[0] == option && board[4] == option && board[8] == option))
		return true;

	return false;
}
bool checkTie()
{
	int tie = 0;
	for(int i = 0; i!= 9; i++)
	{
		if(board[i] != ' ')
		{
			tie++;
		}
	}
	if(tie == 9) return true;
	
	return false;
}
void Win(void)
{
	pp2d_draw_textf(10,10, 10, 40, RGBA8(0xFF,0xFF,0xFF,0xFF),"Player %d Wins!",game.chance == 1? 2 : 1);
}

void topTie(void)
{
	pp2d_draw_text(10,10, 10, 40, RGBA8(0xFF,0xFF,0xFF,0xFF),"It's a Tie!!");
}

void topShowPlayerChance(void)
{
	pp2d_draw_textf(10,10, 10, 40, RGBA8(0xFF,0xFF,0xFF,0xFF),"Player %d's chance", game.chance);
}

void OptAccordingTotouch(touchPosition *touch)
{
	int tx=touch->px;
	int ty=touch->py;
	int w = 75;
	int h = 75;
	int i = 0;
	for(i=0; i<=8; i++)
	{
		
		if (tx > arr[i][0] && tx < arr[i][0]+w && ty > arr[i][1] && ty < arr[i][1]+h) 
		{
			if(board[i] == ' ')
			{
				char option = (game.chance == 1)? game.player1opt : game.player2opt;
				board[i] = option;
				gui.topFunc = topShowPlayerChance;
				if(checkWin(i, option))
				{
					gui.topFunc = Win;
				}
				else if(checkTie() == true)
				{
					gui.topFunc = topTie;
				}
				game.chance = (game.chance == 1) ? 2 : 1;	
				break;
			}	
		}
	
	}
	
}

u8 tex[2];
void botdrawBoard(void)
{
	pp2d_texture_select(tex[0], 0, 0);
	pp2d_texture_flip(HORIZONTAL);
	pp2d_texture_depth(0.7f);
	pp2d_texture_draw();
	pp2d_draw_rectangle(120, 0, 2, 240, RGBA8(0xFF,0x00,0x00,0xFF));
	pp2d_draw_rectangle(200, 0, 2, 240, RGBA8(0xFF,0x00,0x00,0xFF));
	pp2d_draw_rectangle(40, 80, 240 , 2, RGBA8(0xFF,0x00,0x00,0xFF)); 
	pp2d_draw_rectangle(40, 160, 240, 2, RGBA8(0xFF,0x00,0x00,0xFF));
	for(int i=0; i<=8; i++)
	{
		pp2d_draw_textf(arr[i][0],arr[i][1], 1.0f, 1.0f, RGBA8(0x0,0x0,0x0,0xFF), "%c", board[i]);			
	}
}
void topDrawBoard(void)
{
	pp2d_draw_textf_center(GFX_TOP, 110, 1.0f, 1.0f, RGBA8(0xFF, 0x00, 0x00, 0xFF), "Welcome to game %s!", username);
	//pp2d_draw_textf(10,10, RGBA8(0x0,0x0,0x0,0xFF),10,"Player 1 you have %c as your option, Player 2 you have %c", 'X', 'O');			
}

void generate_random_x_0(void)
{
	 int value = rand() % 2;
	 game.player1opt = (value==1) ? 'X' : 'O';
	 game.player2opt = (value==1) ? 'O' : 'X';
}
char *getUsername()
{
	cfguInit();
	u8 * outdata = malloc(0x1C);
	u8 * outdata2 = malloc(0x1C);
	CFGU_GetConfigInfoBlk2 (0x1C, 0x000A0000, outdata);
	utf16_to_utf8(outdata2,(u16*)outdata,0x1C);
	cfguExit();
	return (char *)outdata2;
}

int main()
{
	runThread = true;
	srand(time(NULL));
	game.chance = 1;
	username = malloc(100 * sizeof(char*));
	username = getUsername();
	generate_random_x_0();
	gui.topFunc = topDrawBoard;
	gui.botFunc = botdrawBoard;
	Thread thread = threadCreate(guithread,NULL,(8*1024),0x24,-2,true);
	hidInit();
	//tex1 = sf2d_create_texture_mem_RGBA8(table.pixel_data, table.width, table.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
	pp2d_load_texture_png_memory(tex[0], table_png, table_png_size);
	//pp2d_load_texture_png_memory(tex[1], paper_png, paper_png_size);
	//tex2 = sf2d_create_texture_mem_RGBA8(paper.pixel_data, paper.width, paper.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
	touchPosition touch;
	while (aptMainLoop()) {

		hidScanInput();

		if (hidKeysHeld() & KEY_START) {
			break;
		}
		hidTouchRead(&touch);
		
			OptAccordingTotouch(&touch);
			
	}
	hidExit();
	return 0;
}
