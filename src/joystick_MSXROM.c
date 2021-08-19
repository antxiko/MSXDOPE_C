/* =============================================================================
   Joystick MSX ROM SDCC Library (fR3eL)
   Version: 1.2 (11/03/2018)
   Author: mvac7
   Architecture: MSX
   Format: C Object (SDCC .rel)
   Programming language: C and Z80 assembler
   Description:
     Open Source library with basic functions for reading joystick controllers 
     or cursor keys of MSX computers. 
    
   History of versions:
     v1.2 (11/03/2018) current version
     v1.1 (03/03/2017) 
     v1.0 (01/02/2016)
============================================================================= */


#include "../include/joystick.h"
#include "../include/msxBIOS.h"


/* =============================================================================
STICK
 Description: Returns the joystick status
 
 Input    : [char] joy port: 
                  0=cursor keys 
                  1=joystick A 
                  2=joystick B
 Output   : [char]
                  0=inactive
                  1=up
                  2=up & right
                  3=right
                  4=down & right
                  5=down
                  6=down & left
                  7=left
                  8=up & left 
============================================================================= */
char STICK(char joy) __naked
{
joy;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   A,4(IX)
    
  call GTSTCK
   
  ld   L,A
  
  pop  IX
  ret  
__endasm;
}


/* =============================================================================
 STRIG
  
 Description: Returns current trigger status
            
 Input    : [char] 
                    0 - space key
                    1 - button 1 joy A
                    2 - button 1 joy B
                    3 - button 2 joy A
                    4 - button 2 joy B
                    
 Output   : [char] status:
                    0 = inactive
                   -1 = when the space key or the joystick/mouse button is 
                        pressed down 
============================================================================= */
signed char STRIG(char joy) __naked
{
joy;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   A,4(IX)

  call GTTRIG
    
  ld   L,A
  
  pop  IX
  ret
__endasm;
}