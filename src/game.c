#include "../include/newTypes.h"
#include "../include/msxBIOS.h"
#include "../include/msxSystemVars.h"

#include "../include/VDP_TMS9918A.h"
#include "joystick_MSXROM.c"
#include "VDP_SPRITES_S_MSXROM.c"


#include <stdio.h>


#define  HALT __asm halt __endasm   //wait for the next interrupt

//  definition of functions  ---------------------------------------------------
void WAIT(uint cicles);
void LOCATE(char x, char y);
char PEEK(uint address);
void POKE(uint address, char value);

void CLS();

void VPRINT(char column, char line, char* text);  //print in screen 1 or 2
void VPRINT0(char column, char line, char* text);
void VPOKEARRAY(uint vaddr, char* text);
void VPRINTNUMBER(char column, char line, char pLength, uint pNumber);
void printTrig(signed char value);


void DRUGS();
void COST(char column, int barrio);
void BARRIO();
void STOCK();
void VIAJAR();
void COMERCIAR();
char getR();

// constants  ------------------------------------------------------------------
const char bars[][15] = {"Baraka","Lutxana","Leioa","Erandio","Lekeitio","Oslo"};
const char drugs[][15] = {"Pegamento","Kifi","Hachis","Marihuana","Cocaina","MDMA", "LSD", "Kriptonita"};
const char posX[]={0x15,0x6C,0xAC};
const char posY[]={0x1E,0x26,0x2E,0x36,0x3E,0x46,0x4E,0x56,};

const int disp[][8] = {
  { 9, 12, 18, 30,105, 280, 1010, 2100},
  { 11, 14, 19, 32,95, 310, 1050, 2050},
  { 10, 15, 18, 35,105, 290, 1080, 1990},
  { 10, 12, 21, 33,100, 305, 1030, 2030},
  { 8, 13, 20, 28,90, 310, 1020, 1980},
  { 10, 20, 19, 40,100, 300, 1000, 1950}
  };
  

static unsigned long int next;

//  definition of variables  ---------------------------------------------------

int CURSOR = 0;
int r = 1;
int stock[] = { 0, 0, 0, 0, 0, 0, 0, 0};
int camello[] = { 0, 0, 0, 0, 0, 0, 0, 0};

int dia = 31;
int dinero = 3000;
int deuda = 3000;
int bar = 0;
char SEED;




// Project: tMSgFX_def_tilesets
// tileset pattern
// Tiles range: 32 to 127
// Size= 768
const char TILESET_B0[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x0C,0x0C,0x00,0x0C,0x00,
0x00,0x14,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x14,0x3E,0x14,0x3E,0x14,0x00,0x00,
0x08,0x3E,0x68,0x3E,0x0B,0x3E,0x08,0x00,0x00,0x33,0x36,0x0C,0x1B,0x33,0x00,0x00,
0x00,0x38,0x6C,0x3D,0x66,0x3D,0x00,0x00,0x00,0x18,0x30,0x00,0x00,0x00,0x00,0x00,
0x06,0x0C,0x0C,0x0C,0x0C,0x0C,0x06,0x00,0x30,0x18,0x18,0x18,0x18,0x18,0x30,0x00,
0x00,0x5A,0x3C,0x7E,0x3C,0x5A,0x00,0x00,0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00,
0x00,0x00,0x00,0x00,0x18,0x18,0x10,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x06,0x0C,0x18,0x30,0x60,0x00,0x00,
0x00,0x3E,0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x18,0x38,0x18,0x18,0x3C,0x00,0x00,
0x00,0x7E,0x03,0x3E,0x60,0x7F,0x00,0x00,0x00,0x7E,0x03,0x1E,0x03,0x7E,0x00,0x00,
0x00,0x63,0x63,0x7F,0x03,0x03,0x00,0x00,0x00,0x7F,0x60,0x7E,0x03,0x7E,0x00,0x00,
0x00,0x3E,0x60,0x7E,0x63,0x3E,0x00,0x00,0x00,0x7F,0x03,0x06,0x0C,0x0C,0x00,0x00,
0x00,0x3E,0x63,0x3E,0x63,0x3E,0x00,0x00,0x00,0x3E,0x63,0x3F,0x03,0x3E,0x00,0x00,
0x00,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x18,0x00,0x18,0x18,0x10,0x00,
0x00,0x0C,0x18,0x30,0x18,0x0C,0x00,0x00,0x00,0x00,0x3C,0x00,0x3C,0x00,0x00,0x00,
0x00,0x30,0x18,0x0C,0x18,0x30,0x00,0x00,0x00,0x1E,0x33,0x06,0x0C,0x00,0x0C,0x00,
0x3E,0x41,0x5D,0x55,0x5F,0x40,0x3E,0x00,0x00,0x3E,0x63,0x7F,0x63,0x63,0x00,0x00,
0x00,0x7E,0x63,0x7E,0x63,0x7E,0x00,0x00,0x00,0x3F,0x60,0x60,0x60,0x3F,0x00,0x00,
0x00,0x7E,0x63,0x63,0x63,0x7E,0x00,0x00,0x00,0x7F,0x60,0x7C,0x60,0x7F,0x00,0x00,
0x00,0x7F,0x60,0x7C,0x60,0x60,0x00,0x00,0x00,0x3F,0x60,0x67,0x63,0x3F,0x00,0x00,
0x00,0x63,0x63,0x7F,0x63,0x63,0x00,0x00,0x00,0x0C,0x0C,0x0C,0x0C,0x0C,0x00,0x00,
0x00,0x0C,0x0C,0x0C,0x0C,0x38,0x00,0x00,0x00,0x63,0x66,0x7C,0x66,0x63,0x00,0x00,
0x00,0x60,0x60,0x60,0x60,0x7F,0x00,0x00,0x00,0x63,0x77,0x7F,0x6B,0x63,0x00,0x00,
0x00,0x63,0x73,0x7B,0x6F,0x67,0x00,0x00,0x00,0x3E,0x63,0x63,0x63,0x3E,0x00,0x00,
0x00,0x7E,0x63,0x63,0x7E,0x60,0x00,0x00,0x00,0x3E,0x63,0x6B,0x66,0x3D,0x00,0x00,
0x00,0x7E,0x63,0x63,0x7E,0x63,0x00,0x00,0x00,0x3F,0x60,0x3E,0x03,0x7E,0x00,0x00,
0x00,0x7E,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x63,0x63,0x63,0x63,0x3E,0x00,0x00,
0x00,0x63,0x63,0x63,0x36,0x1C,0x00,0x00,0x00,0x63,0x6B,0x7F,0x77,0x63,0x00,0x00,
0x00,0x63,0x36,0x1C,0x36,0x63,0x00,0x00,0x00,0x66,0x66,0x3C,0x18,0x18,0x00,0x00,
0x00,0x7E,0x0C,0x18,0x30,0x7E,0x00,0x00,0x0E,0x0C,0x0C,0x0C,0x0C,0x0C,0x0E,0x00,
0x00,0x60,0x30,0x18,0x0C,0x06,0x00,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x38,0x00,
0x10,0x38,0x6C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x03,0x3F,0x63,0x7F,0x00,0x00,
0x60,0x7E,0x63,0x63,0x63,0x7E,0x00,0x00,0x00,0x3F,0x60,0x60,0x60,0x3F,0x00,0x00,
0x03,0x3F,0x63,0x63,0x63,0x3F,0x00,0x00,0x00,0x3E,0x63,0x7F,0x60,0x3E,0x00,0x00,
0x00,0x3E,0x60,0x78,0x60,0x60,0x00,0x00,0x00,0x3E,0x63,0x63,0x3F,0x03,0x3E,0x00,
0x60,0x7E,0x63,0x63,0x63,0x63,0x00,0x00,0x0C,0x00,0x0C,0x0C,0x0C,0x0C,0x00,0x00,
0x0C,0x00,0x0C,0x0C,0x0C,0x0C,0x38,0x00,0x00,0x60,0x66,0x7C,0x66,0x63,0x00,0x00,
0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x7E,0x6B,0x6B,0x6B,0x6B,0x00,0x00,
0x00,0x7E,0x63,0x63,0x63,0x63,0x00,0x00,0x00,0x3E,0x63,0x63,0x63,0x3E,0x00,0x00,
0x00,0x7E,0x63,0x63,0x63,0x7E,0x60,0x00,0x00,0x3F,0x63,0x63,0x63,0x3F,0x03,0x00,
0x00,0x3F,0x60,0x60,0x60,0x60,0x00,0x00,0x00,0x3F,0x60,0x3E,0x03,0x7E,0x00,0x00,
0x18,0x3E,0x18,0x18,0x18,0x0E,0x00,0x00,0x00,0x63,0x63,0x63,0x63,0x3E,0x00,0x00,
0x00,0x63,0x63,0x63,0x36,0x1C,0x00,0x00,0x00,0x63,0x6B,0x6B,0x6B,0x36,0x00,0x00,
0x00,0x66,0x3C,0x18,0x3C,0x66,0x00,0x00,0x00,0x63,0x63,0x63,0x3F,0x03,0x3E,0x00,
0x00,0x7E,0x0C,0x18,0x30,0x7E,0x00,0x00,0x06,0x0C,0x0C,0x18,0x0C,0x0C,0x06,0x00,
0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00,0x30,0x18,0x18,0x0C,0x18,0x18,0x30,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

char _LineLength;  //sprites per line. TMS9918=4; V9938=8

char spr_posX[8];
char spr_posY[8];

char dir=0;
signed char button=0;

const char sprcol[8]={12,2,3,7,6,8,9,14};

const char SPRITE_DATA8x8[]={
60,66,165,129,165,153,66,60,
60,126,219,255,255,219,102,60,
108,254,254,254,124,56,16,0,
16,56,124,254,124,56,16,0,
16,56,84,254,84,16,56,0,
16,56,124,254,254,16,56,0,
0,0,0,24,24,0,0,0,
255,255,255,231,231,255,255,255
};



void main(void)
{
  int s;
  for (s=0; s<8; ++s){
    stock[s] = 0;
  }
  for (s=0; s<8; ++s){
    camello[s] = 15;
  }
  COST(15,bar);
  bar = 0;
  dia = 31;
  dinero = 3000;
  deuda = 3000;


  setSprites8x8Patterns();
  initSprites();
   
  
  COLOR(15,1,1); 
  SCREEN(1);
  BARRIO(); 
}

void BARRIO()
{
  int CURSOR = 0;
  CLS();

  VPRINT (0,0, "Barrio: ");
  VPRINT (8,0, bars[bar]);
  VPRINT (20,0, "Dia: ");
  VPRINTNUMBER(24,0,2, dia);
  VPRINT (0,1, "Dinero: ");
  VPRINTNUMBER (8,1, 5, dinero);
  VPRINT (16,1, "Deuda: ");
  VPRINTNUMBER(22,1,5, deuda);

  VPRINT (4,22, "COMERCIAR");
  VPRINT (15,22,"VIAJAR");
  VPRINT (23,22,"STOCK");
  DRUGS ();
  char cr = 15;
  int b = bar;
  int c;

  COST(cr,b);

    while(1)
    {
    HALT;
    //------------------------- cursor keys
    dir = STICK(CURSORKEYS);
    VPRINTNUMBER(1,14,1,dir);
    
    button=STRIG(KEYBOARD_BUTTON);
    printTrig(button);

    PUTSPRITE(0, posX[CURSOR], 174, 8, 7);
    if (dir == 3){
      CURSOR = CURSOR + 1;
        if (CURSOR == 3){
          CURSOR=2;
        }
      VPRINT(10,17,"HAS PULSADO DRCH");
      WAIT(15);
      VPRINT(10,17,"                ");
      VPRINTNUMBER(24,14,1,CURSOR);
      char r;
      r = getR();
      VPRINTNUMBER(0,3,4, r);
      PUTSPRITE(0, posX[CURSOR], 174, 8, 7);
    }
    if (dir == 7){
      CURSOR = CURSOR - 1;
        if (CURSOR < 0){
          CURSOR=0;
        }
      VPRINT(10,17,"HAS PULSADO IZDA");
      WAIT(15);
      VPRINT(10,17,"                ");
      VPRINTNUMBER(24,14,1,CURSOR);
      PUTSPRITE(0, posX[CURSOR], 174, 8, 7);
    }
    if (button < 0 && CURSOR == 2){
      VPRINT(10,17,"HAS PULSADO STOCK");
      WAIT(15);
      VPRINT(10,17,"                 ");
      STOCK();
    }
    if (button < 0 && CURSOR == 1){
      VPRINT(10,17,"HAS PULSADO VIAJAR");
      WAIT(15);
      VPRINT(10,17,"                 ");
      VIAJAR();
    }
     if (button < 0 && CURSOR == 0){
      VPRINT(10,13,"HAS PULSADO COMERCIAR");
      WAIT(15);
      VPRINT(10,13,"                     ");
      COMERCIAR();
    }
  }
  //return ;
}

void COMERCIAR()
{ 
  CURSOR = 0;

  CLS();
  DRUGS();
  printCAMEL();
  char cr = 21;
  int b = bar;
  COST(cr,b);
  WAIT(15);

  while(1)
    {
    HALT;
    //------------------------- cursor keys
    dir = STICK(CURSORKEYS);
    VPRINTNUMBER(1,14,1,dir);
    
    button=STRIG(KEYBOARD_BUTTON);
    printTrig(button);

    PUTSPRITE(0,20,posY[CURSOR], 8, 7);
    
    if (dir == 3){
      if (stock[CURSOR] > 0)
      {
        stock[CURSOR] = stock[CURSOR]- 1;
        camello[CURSOR] = camello[CURSOR] +1;
        printCAMEL();
      }
      VPRINT(10,17,"HAS PULSADO DRCH");
      WAIT(15);
      VPRINT(10,17,"                ");
    }
    if (dir == 7){
      if (camello[CURSOR] > 0)
      {
        stock[CURSOR] = stock[CURSOR]+ 1;
        camello[CURSOR] = camello[CURSOR] -1;
        printCAMEL();
      }
      VPRINT(10,17,"HAS PULSADO IZDA");
      WAIT(15);
      VPRINT(10,17,"                ");
    }
    if (dir == 1){
      CURSOR = CURSOR - 1;
        if (CURSOR < 0){
          CURSOR = 0;
        }
      VPRINT(10,17,"HAS PULSADO ARRIBA");
      WAIT(15);
      VPRINT(10,17,"                  ");
      VPRINTNUMBER(24,14,1,CURSOR);
      PUTSPRITE(0,20,posY[CURSOR], 8, 7);
    }
    if (dir == 5){
      CURSOR = CURSOR + 1;
        if (CURSOR > 7){
          CURSOR=7;
        }
      VPRINT(10,17,"HAS PULSADO ABAJO");
      WAIT(15);
      VPRINT(10,17,"                 ");
      VPRINTNUMBER(24,14,1,CURSOR);
      PUTSPRITE(0,20,posY[CURSOR], 8, 7);
    }
    if (button < 0)
    {
      VPRINT(10,17,"HAS PULSADO SPACIO");
      WAIT(15);
      VPRINT(10,17,"                  ");
      //bar = CURSOR;
      BARRIO();
    }
  }
}
void STOCK()
{
  CLS();
  DRUGS();
  int c;
  for (c = 0; c < 8; ++c)
  {
  VPRINTNUMBER(15,4+c,2,stock[c]);
  }
  WAIT(15);

  while(1)
    {
    HALT;
    //------------------------- cursor keys
    dir = STICK(CURSORKEYS);
    VPRINTNUMBER(1,14,1,dir);
    
    button=STRIG(KEYBOARD_BUTTON);
    printTrig(button);

    PUTSPRITE(0, posX[CURSOR], 174, 8, 7);
    if (button < 0)
    {
      VPRINT(10,17,"HAS PULSADO SPACIO");
      WAIT(15);
      VPRINT(10,17,"                  ");
      BARRIO();
    }
  }
}

void VIAJAR()
{
  CLS();
  CURSOR = 0;
  int d;
  for (d = 0; d < 6; ++d)
  {
  VPRINT(10,4+d,bars[d]);
  }
  VPRINT(10,10,"Hospital");
  VPRINT(10,11,"Prestamista");
  WAIT(15);

  while(1)
    {
    HALT;
    //------------------------- cursor keys
    dir = STICK(CURSORKEYS);
    VPRINTNUMBER(1,14,1,dir);
    
    button=STRIG(KEYBOARD_BUTTON);
    printTrig(button);

    PUTSPRITE(0,70,posY[CURSOR], 8, 7);
    
    if (dir == 1){
      CURSOR = CURSOR - 1;
        if (CURSOR < 0){
          CURSOR = 0;
        }
      VPRINT(10,17,"HAS PULSADO ARRIBA");
      WAIT(15);
      VPRINT(10,17,"                  ");
      VPRINTNUMBER(24,14,1,CURSOR);
      PUTSPRITE(0,70,posY[CURSOR], 8, 7);
    }
    if (dir == 5){
      CURSOR = CURSOR + 1;
        if (CURSOR > 7){
          CURSOR=7;
        }
      VPRINT(10,17,"HAS PULSADO ABAJO");
      WAIT(15);
      VPRINT(10,17,"                 ");
      VPRINTNUMBER(24,14,1,CURSOR);
      PUTSPRITE(0,70,posY[CURSOR], 8, 7);
    }
    if (button < 0)
    {
      VPRINT(10,17,"HAS PULSADO SPACIO");
      WAIT(15);
      VPRINT(10,17,"                  ");
      char r;
      r = getR();
      if (r<70 && r>0)
      {
        CLS();
        VPRINT(0,0,"NO HA PASADO NADA");
        WAIT(50);
      }
      if (r<89 && r>71)
      {
        CLS();
        VPRINT(0,0,"JONKI!!!!");
        WAIT(50);
      }
      if (r<99 && r>91)
      {
        CLS();
        VPRINT(0,0,"POLICIA!!!!");
        WAIT(50);
      }
      if (r<115 && r>100)
      {
        VPRINT(0,0,"OVERFLOW");
        WAIT(50);
      }
      if (r<128 && r>114)
      {
        VPRINT(0,0,"UNDERFLOW");
        WAIT(50);
      }
      bar = CURSOR;
      dia = dia - 1;
      deuda = (deuda+200);
      BARRIO();
    }
  }
}
void DRUGS(){
  int d;
  for (d = 0; d < 8; ++d){
  VPRINT(4,4+d,drugs[d]);
  }
}

void COST(char column, int barrio){
  int c;
  for (c = 0; c < 8; ++c)
  {
  VPRINTNUMBER(column,4+c,5,disp[barrio][c]);
  }
}

void printCAMEL()
{
  int c;
  for (c = 0; c < 8; ++c)
  {
  VPRINTNUMBER(15,4+c,2,stock[c]);
  }
  for (c = 0; c < 8; ++c)
  {
  VPRINTNUMBER(18,4+c,2,camello[c]);
  }
  WAIT(15);
}


/* =============================================================================
   Set a position the cursor in to the specified location
   Posiciona el cursor hasta la ubicacion especificada
   x(byte) - column (0 to 31 or 39)
   y(byte) - line   (0 to 23)
============================================================================= */
void LOCATE(char x, char y)
{
x;y;
__asm
  push IX
  ld   IX,#0
  add  IX,SP

  ld   A,4(IX) ;x
  inc  A       ;incrementa las posiciones para que se situen correctamente en la pantalla
  ld   H,A
  ld   A,5(IX) ;y
  inc  A
  ld   L,A

  call POSIT

  pop  IX
__endasm;

}



// Generates a pause in the execution of n interruptions.
// PAL: 50=1second. ; NTSC: 60=1second.
void WAIT(uint cicles)
{
  uint i;
  for(i=0;i<cicles;i++) HALT;
}



char PEEK(uint address)
{
address;
__asm
  push IX
  ld   IX,#0
  add  IX,SP

  ld   L,4(ix)
  ld   H,5(ix)
  ld   A,(hl)

  ld   L,A

  pop  IX
__endasm;
}



void POKE(uint address, char value)
{
address;value;
__asm
  push IX
  ld   IX,#0
  add  IX,SP

  ld   L,4(ix)
  ld   H,5(ix)
  ld   A,6(ix)
  ld   (HL),A

  pop  IX
__endasm;
}



void CLS()
{
  FillVRAM(BASE10, 0x300, 32);
}





//print in screen 1 or 2
void VPRINT(char column, char line, char* text)
{
  uint vaddr = BASE10 + (line*32)+column; // calcula la posicion en la VRAM
  VPOKEARRAY(vaddr, text);
}

//print in screen 0
void VPRINT0(char column, char line, char* text)
{
  uint vaddr = BASE0 + (line*40)+column; // calcula la posicion en la VRAM
  VPOKEARRAY(vaddr, text);
}


void VPOKEARRAY(uint vaddr, char* text)
{
  while(*(text)) VPOKE(vaddr++,*(text++));
}

void VPRINTNUMBER(char column, char line, char pLength, uint pNumber)
{
  uint vaddr = BASE10 + (line*32) + column // calcula la posicion en la VRAM...
                 + pLength - 1; // ...empezando por el dígito más a la derecha
  char length = pLength;
  uint number = pNumber;
  
  // Mientras queden dígitos por pintar...
  while (length) {
  
    // Pinta un dígito
    uint digit = number % 10; // Calcula el último digito (resto de la división entre 10)
    char digitAscii = '0' + ((char) digit); // Convierte el dígito en su código ASCII de número
    VPOKE(vaddr, digitAscii); // Pinta el dígito
    
    // Se mueve al siguiente dígito
    length--; // Descuenta el dígito que acabamos de pintar
    vaddr--; // El siguiente dígito se pintará un caracter más hacia la izquierda
    number = number / 10; // Divide entre 10 para "avanzar" en el número
 }
}


void printTrig(signed char value)
{
  if(value<0) VPRINT(0,15,"-1");
  else VPRINT(0,15," 0");
}

// Copy sprites data from memory to VRAM
void setSprites8x8Patterns()
{
  HALT;
  CopyToVRAM((uint) SPRITE_DATA8x8,BASE14,8*8);
}

void initSprites()
{
  char X=0,Y=3;
  char i=0;
  
  for(i=0;i<8;i++)
  {
    spr_posX[i]=X*32;
    spr_posY[i]=Y*32;
    X++;
    if(X==_LineLength)
    {
      X=0;
      Y++;
    }
  }
}

char getR()
{
__asm

  ld   A,R  
  ld   L,A ;return L
   
__endasm;
}




