/* ============================================================================= 
  VDP SPRITE Small MSX ROM SDCC Library (fR3eL Project)
  Version: 1.2 (22/12/2020)
  Author: mvac7 [mvac7303b@gmail.com]
  Architecture: MSX
  Format: C Object (SDCC .rel)
  Programming language: C and Z80 assembler
  Description:
    Open Source library with functions to directly access to sprites of the 
    TMS9918A.
    
    It uses the functions from the MSX BIOS, so it is designed to create 
    applications in ROM format.
    It's complemented with the TMS9918A MSX ROM Library.
    
    For SDCC 3.9 or higher. 
    
  History of versions:
    v1.2 (22/12/2020) Removed sprite mode initialization functions.
    v1.1 (2/2/2017)                   
============================================================================= */ 

#include "../include/VDP_SPRITES_S.h"

#include "../include/msxSystemVars.h"
#include "../include/msxBIOS.h"




/* =============================================================================
 ClearSprites
 Description: 
             Initialises all sprite data The sprite pattern is cleared to
             null, the sprite number to the sprite plane number, the
             sprite colour to the foregtound colour. The vertical location
             of the sprite is set to 209 (mode 0 to 3) or 217 (mode 4 to 8).
 Input:       -
 Output:      -
============================================================================= */
void ClearSprites()
{
__asm
  call CLRSPR
__endasm;
}    



/* =============================================================================
 PUTSPRITE
 Description: Displays the sprite pattern.
 Input:       [char] sprite plane (0-31) 
              [char] x 
              [char] y
              [char] color (0-15)
              [char] pattern
 Output:      -
============================================================================= */
void PUTSPRITE(char plane, char x, char y, char color, char pattern)
{
plane;x;y;color;pattern;
__asm
  push IX
  ld   IX,#0
  add  IX,SP

  ld   A,4(IX) ;num sprite plane
  call CALATR
  
  ld   A,6(IX) ;y
  call WRTVRM
  
  inc  HL  
  ld   A,5(IX) ;x
  call WRTVRM
  
  inc  HL  
  ld   E,8(IX)
  call SPRPATNOV  ;pattern
  
  inc  HL
  ld   A,7(IX) ;color
  call WRTVRM
  
  pop  IX
  ret
  


; set sprite pattern number
; Multiply * 4 when its a 16x16 sprite.
; E - Sprite Number  
SPRPATNOV:  ;sprite pattern no calc vram address
  call GSPSIZ          ;0x008A get sprite size in bytes 
  cp   #32             ;16x16
  jr   NZ,WRTPAT

  ;IF 16x16
  SLA  E
  SLA  E ;multiply x 4  

WRTPAT:
  LD   A,E
  call WRTVRM
  
  ;ret
__endasm;
}
