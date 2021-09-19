/* =============================================================================
   SDCC VDP PRINT Lib v1.2 (16/April/2021)
   Author: mvac7
   Architecture: MSX
   Format: .rel (SDCC object file)
   Programming language: C
   
   Description:
     Functions Library for display text strings in the graphic modes of the 
     TMS9918A (G1 and G2).
     
   History of versions:
   - v1.2 (16/April/2021) VPRINTN string size control
   - v1.1 (9/Sep/2020) 
   - v1   (4/Feb/2016) First version     
============================================================================= */
#include "../include/VDP_PRINT.h"
#include "../include/VDP_TMS9918A.h"


char CSTATE;



/* =============================================================================
 It provides the address of the video memory map tiles, from the screen position
 indicated.
 Inputs:
   column (byte) 0 - 31
   line (byte) 0 - 23
============================================================================= */
unsigned int GetVRAMaddressByPosition(char column, char line)
{
   return BASE10 + (line*32)+column;
}



/* =============================================================================
 Prints a string at the indicated screen position
 Inputs:
   column (byte) 0 - 31
   line (byte) 0 - 23
   text (char*) string
============================================================================= */
void VPRINT(char column, char line, char* text)
{
  unsigned int vaddr = GetVRAMaddressByPosition(column, line);
  VPrintString(vaddr, text);
}

/* =============================================================================
 Prints a string at the indicated screen position
 Inputs:
   column (byte) 0 - 31
   line (byte) 0 - 23
   text (char*) string
   length (uint) length of the string to print
============================================================================= */
void VPRINTN(char column, char line, char* text, unsigned int length)
{
  unsigned int vaddr = GetVRAMaddressByPosition(column, line);
  //CopyToVRAM((unsigned int) text, vaddr, length);
  while(*(text) && length-->0)  VPOKE(vaddr++,*(text++));

}



/* =============================================================================
 Dump the contents of an array of char in a position shown in the video memory
 Inputs:
   vaddr (uint) 
   text (char*) array
============================================================================= */
void VPrintString(unsigned int vaddr, char* text)
{
  char length=0;
  while(text[length]) length++;
  //while(*(text)) VPOKE(vaddr++,*(text++));
  CopyToVRAM((unsigned int) text, vaddr, length);
}


/* =============================================================================
   VPrintNumber
   Prints a number at the specified position on the screen.
   
   Inputs:
     [char] column 0 - 31
     [char] line   0 - 23
     [unsigned int] number
     [char] length
============================================================================= */
void VPrintNumber(char column, char line, unsigned int value, char length)
{
  unsigned int vaddr = GetVRAMaddressByPosition(column, line);

  VPrintNum(vaddr, value, length);
}


/* =============================================================================
   VPrintNum
   Prints a number at the specified position on the screen.
   
   Inputs:
     [unsigned int] VRAM address in Pattern Name Table.
     [unsigned int] number
     [char] length
============================================================================= */
void VPrintNum(unsigned int vaddr, unsigned int value, char length)
{
  char pos=5-length;
  char text[]="     ";

  num2Dec16(value, text); 
  
  //while (length-->0){ VPOKE(vaddr++,text[pos++]);}
  CopyToVRAM((unsigned int) text + (5-length), vaddr, length);
}



/* =============================================================================
 16-bit Integer to ASCII (decimal)
 original code by baze http://baze.sk/3sc/misc/z80bits.html#5.1
 (update) Add functionality to replace leading zeros by spaces.  
 Input: HL = number to convert, DE = location of ASCII string
 Output: ASCII string at (DE)
============================================================================= */
void num2Dec16(unsigned int aNumber, char *address) __naked
{
  aNumber;
  address;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   L,4(ix)
  ld   H,5(ix)
  
  ld   E,6(ix)
  ld   D,7(ix)
  
  ld   A,#32         ;ASCII value for space
  ld   (#_CSTATE),A
  	
  ld   BC,#-10000
  call $Num1
  ld   BC,#-1000
  call $Num1
  ld   BC,#-100
  call $Num1
  ld   C,#-10
  call $Num1
  ld   C,B
  call $Num1

  ;END
  pop  IX
  ret
    
$Num1:	
  ld   A,#47 ; 0 ASCII char - 1
   
$Num2:	
  inc  A
  add  HL,BC
  jr   C,$Num2
  
  sbc  HL,BC
  
  cp   #48       
  jr   NZ,$Num3  ;if A!=0 then goto $Num3
  
  ld   A,(#_CSTATE)
  jr   $Num4


$Num3:
  push AF
  ld   A,#48  ;ASCII value for 0
  ld   (#_CSTATE),A
  pop  AF	
	
$Num4:	
  ld   (DE),A
  inc  DE
  	
  ret
  
__endasm;
}
