#include "../include/newTypes.h"
#include "../include/msxSystemVars.h"
#include "../include/msxBIOS.h"
#include "../include/VDP_TMS9918A.h"
#include "../include/interruptM1_Hooks.h"
#include "../include/AY38910BF.h"
#include "../include/PT3Player.h"
#include "../include/PT3player_NoteTable2.h"


#include "joystick_MSXROM.c"
#include "VDP_SPRITES_S_MSXROM.c"
//#include "VDP_PRINT.c"
#include "musicas.h"


#define  HALT __asm halt __endasm   //wait for the next interrupt


char _LineLength;  //sprites per line. TMS9918=4; V9938=8

uint song;

void main(void)

{
  song = (unsigned int) SONG00;

Player_Init();
Player_InitSong(song, (unsigned int) NT2, OFF);

while(1)
    {
    HALT;
    
    PlayAY();
    Player_Decode();
  
  }
}


