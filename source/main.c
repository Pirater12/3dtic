#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <stdbool.h>
#include <stdlib.h>
#include "guithread.h"
#include "font_ttf.h"

extern const struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel;
  unsigned char	 pixel_data[];
} table;

extern const struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel;
  unsigned char	 pixel_data[];
} paper;

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
	sftd_draw_textf(font,10,10, RGBA8(0xFF,0xFF,0xFF,0xFF),40,"Player %d Wins!",game.chance == 1? 2 : 1);
}

void topTie(void)
{
	sftd_draw_text(font,10,10, RGBA8(0xFF,0xFF,0xFF,0xFF),40,"It's a Tie!!");
}

void topShowPlayerChance(void)
{
	sftd_draw_textf(font,10,10, RGBA8(0xFF,0xFF,0xFF,0xFF),40,"Player %d's chance", game.chance);
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

sf2d_texture *tex1 = NULL, *tex2 = NULL;
void botdrawBoard(void)
{
	sf2d_draw_texture(tex1,0,0);
	sf2d_draw_texture(tex2,40,0);
	sf2d_draw_line (120,0,120,240,2,RGBA8(0xFF,0x00,0x00,0xFF));
	sf2d_draw_line (200,0,200,240,2,RGBA8(0xFF,0x00,0x00,0xFF));
	sf2d_draw_line (40,80,280,80,2,RGBA8(0xFF,0x00,0x00,0xFF)); 
	sf2d_draw_line (40,160,280,160,2,RGBA8(0xFF,0x00,0x00,0xFF));	
	for(int i=0; i<=8; i++)
	{
		sftd_draw_textf(font,arr[i][0],arr[i][1], RGBA8(0x0,0x0,0x0,0xFF),80,"%c",board[i]);			
	}
}
void topDrawBoard(void)
{
	sftd_draw_textf(font,10,10, RGBA8(0xFF,0xFF,0xFF,0xFF),20,"Welcome to game %s", username);
	//sftd_draw_textf(font,10,10, RGBA8(0x0,0x0,0x0,0xFF),10,"Player 1 you have %c as your option, Player 2 you have %c", 'X', 'O');			
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
	gui.botFunc = botdrawBoard;//getUsername();
	Thread thread = threadCreate(guithread,NULL,(8*1024),0x24,-2,true);
	hidInit();
	tex1 = sf2d_create_texture_mem_RGBA8(table.pixel_data, table.width, table.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
	tex2 = sf2d_create_texture_mem_RGBA8(paper.pixel_data, paper.width, paper.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
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
	sf2d_free_texture(tex2);
	sf2d_free_texture(tex1);
	return 0;
}
