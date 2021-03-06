#include "../include/newTypes.h"
#include "../include/msxSystemVars.h"
//#include "../include/msxBIOS.h"
#include "../include/VDP_TMS9918A.h"
#include "../include/VDP_VPRINT.h"
#include "../include/AY38910BF_S.h"
#include "../include/PT3Player.h"
#include "../include/PT3player_NoteTable2.h"


#include "joystick_MSXROM.c"
#include "VDP_SPRITES_S_MSXROM.c"
#include "graficos.c"
#include "musicas.h"


#define  HALT __asm halt __endasm   //wait for the next interrupt

void WAIT(uint cicles);
//char PEEK(uint address);
void POKE(uint address, char value);
void CLS();
void VPRINT(char column, char line, char* text);
void VPOKEARRAY(uint vaddr, char* text);
void VPRINTNUMBER(char column, char line, char pLength, uint pNumber);
void setSprites8x8Patterns();
void initSprites();

void inicializar();
void menuPrincipal();
void DRUGS();
void BARRIO();
void STOCK();
void VIAJAR();
void COMERCIAR();
void JONKI();
void POLICIA();
void pijo();
void PRECIOS(int barrio);
void SOBORNO();
void hospital();
void prestamista();
void underflow();
void overflow();
void printCAMEL();
char getR();
int ran100();
void calcDeuda();
void stockCamello();
void diaSemana();
void rastro();
void checkSecreta();
void imprimeBarrio(int barrio, int barrioAhora);
void imprimeIcono(char column, char line, char caracter);
void peleaBarra(int tipo);
void resultadoBarra(boolean victoria, int personaje, int vidaMenos);
void intentarEscapar(int tipo);
void disparos();
void rebound();
void ganasJonki();
void checkFin();
void printDatos();
void animaCursor();


const unsigned char bilbao[8][12] = {"Baraka","Lutxana","Leioa","Erandio","Lekeitio","Oslo","Hospital","Prestamista"};
const unsigned char madrid[8][13] = {"Orcasitas","Villaverde","Valdemingome","SanCristobal","Carabanchel","Moraleja","Hospital","Prestamista"};
const unsigned char newyork[8][13] = {"Harlem","Soho","China Town","Jersey","Bronx","Central Park","Hospital","Prestamista"};
const unsigned char drugs[8][12] = {"Pegamento","Kifi","Hachis","Marihuana","Cocaina","MDMA", "LSD", "Kriptonita"};
const unsigned char diasSEMANA[7][10] = {"Lunes","Martes","Miercoles","Jueves","Viernes","Sabado","Domingo"};

const unsigned char posXPOLI[]={0x00,0x39,0x78};
const unsigned char posY[]={0x2E,0x36,0x3E,0x46,0x4E,0x56,0x5E,0x66};
const unsigned char posYHOSPI[]={0x2E,0x36,0x3E,175};
const unsigned char posYpresta[]={0x2E,0x36,0x3E,0x46,0x4E,175};
const unsigned char posYrastro[]={0x2E,0x36,0x3E,175};
const unsigned char posXmenu[]={16,72,128};
const unsigned int rastroDia[] = {2,1,0,4,3,5};

const unsigned int disp[6][8] = {
  { 9, 12, 18, 30,105, 280, 1010, 2100},
  { 11, 14, 19, 32,95, 310, 1050, 2050},
  { 10, 15, 18, 35,105, 290, 1080, 1990},
  { 10, 12, 21, 33,100, 305, 1030, 2030},
  { 8, 13, 20, 28,90, 310, 1020, 1980},
  { 10, 20, 19, 40,100, 300, 1000, 1950}
  };

const int precioVAR[8] = { 1,2, 3, 5, 11, 18, 36, 60};
const int camelloVAR[8] = { 4,5, 6, 10, 12, 16, 23, 35};

static unsigned long int next;

unsigned int r;
unsigned int stock[8];
unsigned int camello[8];
unsigned int precioTEMP[8];
unsigned int precioBARRIO[8];
unsigned int dia;
unsigned int cientos;
unsigned int miles;
unsigned long dinero;
unsigned int deuda;
unsigned int vida;
unsigned int bar;
unsigned int deudaCiudad;
char SEED;
unsigned int municion;
unsigned int fecha;
unsigned int barrioPartida;
unsigned int randomSecreta;
unsigned int limiteStock;
unsigned int compras;
signed int curMovX;

boolean pistola;
boolean navaja;
boolean cadena;
boolean victoriaBarra;
boolean botiquin;
boolean soplon;
boolean mochila;
boolean rastroOk;
boolean secretaPosible;
boolean haySecreta;
boolean subiendo;

const unsigned char INTRO[]={
0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1,
0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0,
0x0, 0x1, 0x0, 0x1,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, 0x0, 0x1, 0x0, 0x1,
0x1, 0x0,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, 0x4, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, 0x1, 0x0,
0x0, 0x1,0x20,0x20,0x20,0x20,0x20,0x20, 0x2,0x20, 0x5,0x20,0x20,0x20,0x20,0x20, 0x5, 0xc, 0xc, 0xd,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, 0x0, 0x1,
0x1, 0x0,0x20,0x20,0x20,0x20,0x20,0x20, 0x2, 0x3, 0x6, 0x3, 0x3, 0x3, 0x3, 0x3, 0x6, 0xc, 0xc, 0xd, 0xe, 0xf, 0xf, 0xf, 0xf, 0xf,0x20,0x20,0x20,0x20, 0x1, 0x0,
0x0, 0x1,0x20,0x20,0x20,0x20,0x20,0x20, 0x2,0x20, 0x5,0x20, 0x9, 0xa, 0xb,0x20, 0x5, 0xc, 0xc, 0xd,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, 0x0, 0x1,
0x1, 0x0,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, 0x5, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, 0x1, 0x0,
0x0, 0x1,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x84,0x8f,0x90,0x85,0x20,0x97,0x81,0x92,0x93,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, 0x0, 0x1,
0x1, 0x0,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0xa4,0xaf,0xb0,0xa5,0x20,0xb7,0xa1,0xb2,0xb3,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, 0x1, 0x0,
0x0, 0x1,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, 0x0, 0x1,
0x1, 0x0,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, 0x1, 0x0,
0x0, 0x1,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, 0x0, 0x1,
0x1, 0x0,0x20,0x20,0x20,0x20,0x20,0x90,0x95,0x93,0x88,0x20,0x93,0x90,0x81,0x83,0x85,0x20,0x94,0x8f,0x20,0x93,0x94,0x81,0x92,0x94,0x20,0x20,0x20,0x20, 0x1, 0x0,
0x0, 0x1,0x20,0x20,0x20,0x20,0x20,0xb0,0xb5,0xb3,0xa8,0x20,0xb3,0xb0,0xa1,0xa3,0xa5,0x20,0xb4,0xaf,0x20,0xb3,0xb4,0xa1,0xb2,0xb4,0x20,0x20,0x20,0x20, 0x0, 0x1,
0x1, 0x0,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, 0x1, 0x0,
0x0, 0x1,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, 0x0, 0x1,
0x1, 0x0,0x20,0x20,0x20,0x20,0x20,0xd2,0xd0,0xd2,0xd2,0x20,0x81,0x8e,0x94,0x98,0x89,0x8b,0x8f,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, 0x1, 0x0,
0x0, 0x1,0x20,0x20,0x20,0x20,0x20,0xf2,0xf0,0xf2,0xf2,0x20,0xa1,0xae,0xb4,0xb8,0xa9,0xab,0xaf,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, 0x0, 0x1,
0x1, 0x0,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, 0x1, 0x0,
0x0, 0x1,0x20,0x54,0x48,0x41,0x4e,0x4b,0x53,0x3a,0x41,0x4f,0x52,0x41,0x4e,0x54,0x45,0x2c,0x4e,0x45,0x53,0x54,0x52,0x55,0x4f,0x20,0x26,0x20,0x20,0x20, 0x0, 0x1,
0x1, 0x0, 0x1, 0x0,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x41,0x4c,0x55,0x43,0x41,0x52,0x44,0x58,0x20,0x20,0x20,0x20,0x20,0x20,0x20, 0x1, 0x0, 0x1, 0x0,
0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1,
0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0
};

unsigned char _LineLength;  //sprites per line. TMS9918=4; V9938=8

unsigned char spr_posX[8];
unsigned char spr_posY[8];
unsigned char dir;
signed char button;
const unsigned char sprcol[8]={12,2,3};
const unsigned char SPRITE_DATA8x8[]={
24,60,126,255,60,60,60,60,
24,36,66,145,137,66,36,24,
0,4,254,255,255,254,4,0
};

void main(void)
{
  
  inicializar(); 
}

void inicializar() 
{
  POKE(CLIKSW,0);

int s;
  for (s=0; s<8; ++s){
    stock[s] = 0;
  }
      int a;
        for (a=0;a<8;++a)
          {
         int b;
         int c;
          c = (ran100()/10);
          b = ran100();
          camello[a] = (b / camelloVAR[a]) + c;
          }
  r = 1;
  dir = 0;
  button = 0;
  bar = 0;
  curMovX = 0;
  bar = 0;
  dinero = 3000;
  deuda = 3000;
  vida = 100;
  municion = 0;
  limiteStock = 20;
  compras = 0;
  cientos = 0;
  miles = 3;
  navaja = false;
  cadena = false;
  pistola = false;
  botiquin = false;
  soplon = false;
  mochila = false;
  rastroOk = false;
  secretaPosible = false;
  haySecreta = false;
  subiendo = true;

  setSprites8x8Patterns();
  initSprites();
 
  COLOR(15,1,1);
  SCREEN(2);
  cargaTiles();

  int fecharan;
  fecharan = ((ran100()*7)/100);
  fecha = fecharan;

  Player_Init();
  Player_InitSong((unsigned int) SONG00, (unsigned int) NT2, ON);

  CopyToVRAM((uint) INTRO,BASE10,96*8);

  while(1)
    {
    HALT;

    dir = STICK(CURSORKEYS);
    button=STRIG(KEYBOARD_BUTTON);
    
    PlayAY();
    
  
    if (button < 0)
    {
    CLS();
    Player_Pause();
    WAIT(10);
    menuPrincipal();
    }
    Player_Decode();
  }
}

void BARRIO()
{ 
  char posX[]={0x15,0x6C,0xAC};
  switch (barrioPartida)
  {
  case 1:
  if (dia < 20) 
  {
    secretaPosible = true;
  }
    break;
  case 2:
  if (dia < 80) 
  {
    secretaPosible = true;
  }
  break;
  case 3:
  if (dia < 355) 
  {
    secretaPosible = true;
  }
  default:
    break;
  }
  
  if (soplon == true && haySecreta == true){
    CLS();
    VPRINT(0,0,"CUIDADO, HAY SECRETA!");
    soplon = false;
    haySecreta = false;
    WAIT(200);
  }

  if (bar == 6){
    hospital();
  }
  if (bar == 7){
    prestamista();
  }

  checkFin();
  
  int CURSOR = 0;
  gui();
  printDatos();

  VPRINT (4,22, "COMERCIAR  VIAJAR  STOCK");
  VPRINT (2,18,diasSEMANA[fecha]);

  if (fecha == rastroDia[bar]){
    VPRINT(16,18,"RASTRO");
    rastroOk = true;
  }
  else {
    rastroOk = false;
  }
  
  DRUGS ();
  char cr = 15;
  int b = bar;
  int c;
  int ras = 0;

  for (c = 0; c < 8; ++c)
  {
  VPRINTNUMBER(15,6+c,5,precioBARRIO[c]);
  }
    
 
    while(1)
    {
    HALT;
    PlayAY();
   
    animaCursor();
    PUTSPRITE(0, posX[CURSOR]+curMovX, 174, 8, 2);
    if (ras == 1){
    PUTSPRITE(0, 120+curMovX, 142, 8, 2);
    }

    dir = STICK(CURSORKEYS);
    button=STRIG(KEYBOARD_BUTTON);

    if (dir == 1 && rastroOk == true){
      PUTSPRITE(0, 120+curMovX, 142, 8, 2);
      ras = 1;
    }
    if (dir == 5){
      PUTSPRITE(0, posX[CURSOR]+curMovX, 174, 8, 2);
      ras = 0;
    }
    if (dir == 3){
      CURSOR = CURSOR + 1;
        if (CURSOR == 3){
          CURSOR=2;
        }
        WAIT(10);
    }
    if (dir == 7)
    {
      CURSOR = CURSOR - 1;
        if (CURSOR < 0)
        {
          CURSOR=0;
        }
        WAIT(10);
    }
    if (button < 0 && rastroOk == true && ras == 1)
    {
    PUTSPRITE(0, 250, 250, 8, 2);
    rebound();
    rastro();
    }
    if (button < 0 && CURSOR == 2 && ras == 0)
    {
      PUTSPRITE(0, 250, 250, 8, 2);
      rebound();
      STOCK();
    }
    if (button < 0 && CURSOR == 1 && ras == 0)
    {
      PUTSPRITE(0, 250, 250, 8, 2);
      rebound();
      VIAJAR();
    }
    if (button < 0 && CURSOR == 0 && ras == 0)
    {
    PUTSPRITE(0, 250, 250, 8, 2);
    rebound();
    COMERCIAR();
    }
    Player_Decode();
  }
}

void menuPrincipal(){

 int CURSOR = 0;
 CLS();
 

 //imprimeIcono(0,0,0);
 VPRINT(8,0,"MENU PRINCIPAL");
 VPRINT(8,8,"MSX DOPE WARS");
 VPRINT(9,13,"ELIGE CIUDAD");
 VPRINT(3,15,"BILBAO MADRID NEW YORK");
 PUTSPRITE(0, posXmenu[CURSOR], 118, 8, 2);
 Player_Resume();
 
    while(1)
    {
    HALT;
    
    PlayAY();

    animaCursor();
    PUTSPRITE(0, posXmenu[CURSOR]+curMovX, 118, 8, 2);

    dir = STICK(CURSORKEYS);
    button=STRIG(KEYBOARD_BUTTON);

    if (dir == 3){
      CURSOR = CURSOR + 1;
        if (CURSOR == 3){
          CURSOR=2;
        }
      WAIT(10);
    }
    if (dir == 7)
    {
      CURSOR = CURSOR - 1;
        if (CURSOR < 0)
        {
          CURSOR=0;
        }
        WAIT(10);
    }

    if (button < 0 && CURSOR == 0)
    {
    PUTSPRITE(0, 260, 240, 8, 2);
    WAIT(10);
    dia = 31;
    deuda = 3000;
    deudaCiudad = 4;
    barrioPartida = 1;
    PRECIOS(bar);
    stockCamello();
    rebound();
    BARRIO();
    }
    if (button < 0 && CURSOR == 1)
    {
    PUTSPRITE(0, 260, 240, 8, 2);
    WAIT(10);
    dia = 90;
    deuda = 5000;
    deudaCiudad = 5;
    barrioPartida = 2;
    PRECIOS(bar);
    stockCamello();
    rebound();
    BARRIO();
    }
    if (button < 0 && CURSOR == 2)
    {
    PUTSPRITE(0, 260, 240, 8, 2);
    WAIT(10);
    dia = 365;
    deuda = 8000;
    deudaCiudad = 6;
    barrioPartida = 3;
    PRECIOS(bar);
    stockCamello();
    rebound();
    BARRIO();
    }
    Player_Decode();
  }
}

void COMERCIAR()
{ 
  int CURSOR = 0;
  curMovX = 0;

  CLS();
  gui();
  //CopyToVRAM((uint) GUI,BASE10,96*8);
  DRUGS();
  printCAMEL();
  VPRINT (3,2, "Dinero: ");
  VPRINTNUMBER (11,2, 5, dinero);
  int c;
  for (c = 0; c < 8; ++c)
  {
  VPRINTNUMBER(21,6+c,5,precioBARRIO[c]);
  }
  
  while(1)
    {     
    HALT;
    PlayAY();

    dir = STICK(CURSORKEYS);
    button=STRIG(KEYBOARD_BUTTON);

    animaCursor();
    PUTSPRITE(0,20+curMovX,posY[CURSOR], 8, 2);
    
    if (dir == 3  && stock[CURSOR] > 0){
        if (haySecreta == true){
          PUTSPRITE(0, 260, 240, 8, 2);
          CLS();
          VPRINT(0,0,"ES UN SECRETA!");
          VPRINT(0,0,"DROGAS REQUISADAS Y 3 DIAS DE CARCEL");
          WAIT(200);
          calcDeuda();
          dia = dia - 3;
          haySecreta = false;
          int d;
          for (d=0;d<8;++d)
            {
            stock[d] = 0;
            }
          compras = municion;
          PRECIOS(bar);
          stockCamello();
          diaSemana();
          BARRIO();
        }
        else
        {
        compras = compras - 1;
        stock[CURSOR] = stock[CURSOR]- 1;
        camello[CURSOR] = camello[CURSOR] +1;
        dinero = (dinero + precioBARRIO[CURSOR]);
        //VPRINTNUMBER(0,0,2,compras);
        printCAMEL();
        VPRINT (3,2, "Dinero: ");
        //VPRINTNUMBER (0,0,2,compras);
        VPRINTNUMBER (11,2, 5, dinero);
        WAIT(10);
        }
    }
    if (dir == 7 && camello[CURSOR] > 0){
      if (dinero>precioBARRIO[CURSOR] && compras < limiteStock)
      {
        if (haySecreta == true){
          CLS();
          VPRINT(0,0,"ES UN SECRETA!");
          VPRINT(0,0,"DROGAS REQUISADAS Y 3 DIAS DE CARCEL");
          WAIT(200);
          calcDeuda();
          dia = dia - 3;
          haySecreta = false;
          int d;
          for (d=0;d<8;++d)
            {
            stock[d] = 0;
            }
          PRECIOS(bar);
          stockCamello();
          diaSemana();
          BARRIO();
        }
        else 
        {
        compras = compras + 1;
        stock[CURSOR] = stock[CURSOR]+ 1;
        camello[CURSOR] = camello[CURSOR] -1;
        dinero = (dinero - precioBARRIO[CURSOR]);
        printCAMEL();
        VPRINT (3,2, "Dinero: ");
        VPRINTNUMBER (11,2, 5, dinero);
        WAIT(10);
        }
      }
    }
    if (dir == 1){
      CURSOR = CURSOR - 1;
        if (CURSOR < 0){
          CURSOR = 0;
        }
        WAIT(10);
        
    }
    if (dir == 5){
      CURSOR = CURSOR + 1;
        if (CURSOR > 7){
          CURSOR=7;
        }
        WAIT(10);
    }
    if (button < 0)
    {
      PUTSPRITE(0, 260, 240, 8, 2);
      CLS();
      rebound();
      BARRIO();
    }
    Player_Decode();
  }
}

void STOCK()
{
  PUTSPRITE(0, 260, 240, 8, 2);
  CLS();
  gui();
  //CopyToVRAM((uint) GUI,BASE10,96*8);
  DRUGS();
  VPRINT(4,14,"Municion");
  int c;
  for (c = 0; c < 8; ++c)
  {
  VPRINTNUMBER(15,6+c,2,stock[c]);
  VPRINTNUMBER(15,14,2,municion);
  }
  
  while(1)
    {
    HALT;
    button=STRIG(KEYBOARD_BUTTON);

    if (button < 0)
    {
      CLS();
      rebound();
      BARRIO();
    }
  }
}

void VIAJAR()
{
  int CURSOR=0;
  CLS();
  gui();
  CURSOR = bar;

  int d;
  for (d = 0; d < 6; ++d)
  {
  
  switch (barrioPartida)
  {
  case 1:
    VPRINT(10,6+d,bilbao[d]);
    break;
  case 2:
    VPRINT(10,6+d,madrid[d]);
    break;
  case 3:
    VPRINT (10,6+d,newyork[d]);
    break;
  default:
    break;
  }
  }
  VPRINT(10,12,"Hospital");
  VPRINT(10,13,"Prestamista");

  while(1)
    {
    HALT;
    
    PlayAY();

    dir = STICK(CURSORKEYS);
    button=STRIG(KEYBOARD_BUTTON);
    animaCursor();
    PUTSPRITE(0,70+curMovX,posY[CURSOR], 8, 2);
    
    if (dir == 1){
      CURSOR = CURSOR - 1;
        if (CURSOR < 0){
          CURSOR = 0;
        }
        WAIT(10);
    }
    if (dir == 5){
      CURSOR = CURSOR + 1;
        if (CURSOR > 7){
          CURSOR=7;
        }
        WAIT(10);
    }
    if (button < 0)
    {
      rebound();
      PUTSPRITE(0, 260, 240, 8, 2);
      bar = CURSOR;
      char r;
      r = getR();
      if (r<64){
        dia = dia - 1;
        calcDeuda();
        CLS();
        PRECIOS(bar);
        diaSemana();
        if (CURSOR < 6) {
        checkSecreta();
        }
        BARRIO();
        WAIT(50);
      }
      if (r<78 && r>=64)
      {
        CLS();
        VPRINT(0,0,"JONKI!!!!");
        WAIT(100);
        JONKI();
      }
      if (r<90 && r>=78)
      {
        CLS();
        VPRINT(0,0,"POLICIA!!!!");
        WAIT(100);
        POLICIA();
      }
      if (r<106 && r>=90)
      {
        CLS();
        VPRINT(0,0,"PIJO CON MONO!!!!");
        WAIT(100);
        pijo();
      }
      if (r<118 && r>=106)
      {
        CLS();
        VPRINT(0,0,"OVERFLOW");
        WAIT(100);
        bar = CURSOR;
        dia = dia - 1;
        calcDeuda();
        CLS();
        PRECIOS(bar);
        overflow();
        stockCamello();
        diaSemana();
        checkSecreta();
        BARRIO();
      }
      if (r>=118)
      {
        CLS();
        VPRINT(0,0,"UNDERFLOW");
        WAIT(100);
        bar = CURSOR;
        dia = dia - 1;
        calcDeuda();
        CLS();
        PRECIOS(bar);
        underflow();
        stockCamello();
        diaSemana();
        checkSecreta();
        BARRIO();
      }
    }
  Player_Decode();
  }
}

void DRUGS(){
  int d;
  for (d = 0; d < 8; ++d){
  VPRINT(4,6+d,drugs[d]);
  }
}

void PRECIOS(int barrio){
  int c;
  for (c = 0; c < 8; ++c)
  {
  precioTEMP[c] = disp[barrio][c];
  }
  for (c = 0; c < 8; ++c)
  {
    char r;
    r = ran100();
    if (r<51)
    {
    precioBARRIO[c] = precioTEMP[c] + precioVAR[c];
    int br;
    br = ((ran100()*3)/100)*precioVAR[c];
    precioBARRIO[c] = precioBARRIO[c] + br;
    }
    else
    {
    precioBARRIO[c] = precioTEMP[c] - precioVAR[c];
    int br;
    br = ((ran100()*3)/100)*precioVAR[c];
    precioBARRIO[c] = precioBARRIO[c] - br;  
    }
  }
}

void JONKI () {
  CLS();
  int CURSOR = 0;
  VPRINT(1,3,"ATACAR");
  VPRINT(8,3,"INTENTAR ESCAPAR");
  PUTSPRITE(0, 0, 24, 8, 2);

  while(1)
    {
    HALT;
    dir = STICK(CURSORKEYS);
    
    button=STRIG(KEYBOARD_BUTTON);
    if (dir == 3){
      CURSOR = CURSOR + 1;
        if (CURSOR > 1)
        {
          CURSOR=1;
        }
        WAIT(20);
        PUTSPRITE(0, 56, 24, 8, 2);
    }
    if (dir == 7)
    {
      CURSOR = CURSOR - 1;
        if (CURSOR < 0)
        {
          CURSOR=0;
        }
        WAIT(20);
        PUTSPRITE(0, 0, 24, 8, 2);
    }
    if (button < 0 && CURSOR == 0){
      PUTSPRITE(0, 260, 240, 8, 2);
      CLS();
      VPRINT(0,0,"ATACAS");
      WAIT(50);
      break;
    }
    if (button < 0 && CURSOR == 1)
    {
      PUTSPRITE(0, 260, 240, 8, 2);
      CLS();
      rebound();
      intentarEscapar(0);
    }
  }
  if (pistola == true && municion == 0)
    {
    VPRINT(0,0,"TIENES PISTOLA,PERO NO MUNICION");
    WAIT(200);
    peleaBarra(1);
    }
  if (pistola == true)
  {
    PUTSPRITE(0, 260, 240, 8, 2);
    CLS();
    VPRINT(0,0,"DISPARAS AL JONKI");
    compras = compras - 1;
    municion = municion - 1;
    ganasJonki();
  }
  else {
    peleaBarra(1);
  }
}

void POLICIA() {
  CLS();
  int CURSOR = 0;
  VPRINT(1,3,"ATACAR ESCAPAR SOBORNAR");
  PUTSPRITE(0,posXPOLI[CURSOR], 24, 8, 2);

  while(1)
    {
    HALT;
    dir = STICK(CURSORKEYS);
    button=STRIG(KEYBOARD_BUTTON);
    if (dir == 3)
      {
      CURSOR = CURSOR + 1;
        if (CURSOR > 2)
        {
          CURSOR=2;
        }
        WAIT(20);
        PUTSPRITE(0, posXPOLI[CURSOR], 24, 8, 2);
      }
    if (dir == 7)
      {
      CURSOR = CURSOR - 1;
        if (CURSOR < 0)
        {
          CURSOR=0;
        }
        WAIT(20);
        PUTSPRITE(0, posXPOLI[CURSOR], 24, 8, 2);
      }
    if (button < 0 && CURSOR == 0)
      {
      rebound();
      PUTSPRITE(0, 260, 240, 8, 2);
      CLS();
      VPRINT(0,0,"ATACAS");
      WAIT(50);
      if (pistola == true && municion == 0)
      {
      CLS();
      WAIT(50);
      peleaBarra(2);  
      }
      else if (pistola == true && municion > 0)
        {
        WAIT(50);
        disparos();
        }
        if (pistola == false){
          CLS();
          WAIT(50);
          peleaBarra(2);
        }
      }
    if (button < 0 && CURSOR == 1)
      {
      rebound();
      intentarEscapar(1);
      }
    if (button < 0 && CURSOR == 2)
      {
      CLS();
      rebound();
      SOBORNO();
      }
    }
  }

void SOBORNO() 
{
  int CURSOR = 0;
PUTSPRITE(0, 260, 240, 8, 2);
CURSOR = 0;
VPRINT(0,0,"SOBORNAS");
WAIT(50);
r = ran100();
int rr = ran100();
int rrr = ran100();
int rf = (r + rr) - rrr;
VPRINT(0,1,"EL POLI QUIERE ");
VPRINTNUMBER(16,1,3,rf);
VPRINT(20,1,"DINEROS");
VPRINT(0,3,"LO PAGAS?");
VPRINT(1,4,"SI");
VPRINT(4,4,"NO");
PUTSPRITE(0, 0,32, 8, 2);
  while(1)
    {
    HALT;
    dir = STICK(CURSORKEYS);
    button=STRIG(KEYBOARD_BUTTON);
    if (dir == 3)
    {
      CURSOR = CURSOR + 1;
        if (CURSOR > 1)
        {
          CURSOR=1;
        }
        WAIT(10);
        PUTSPRITE(0, 24, 32, 8, 2);
    }
    if (dir == 7)
    {
      CURSOR = CURSOR - 1;
        if (CURSOR < 0)
        {
          CURSOR=0;
        }
        WAIT(10);
        PUTSPRITE(0, 0, 32, 8, 2);
    }
    if (button < 0 && CURSOR == 0)
      {
        rebound();
        PUTSPRITE(0, 260, 240, 8, 2);
        if (dinero < rf)
        {
          CLS();
          VPRINT(0,1,"SIN DINERO PARA PAGAR, TE REVIENTA");
          WAIT(200);
          dia = dia - 3;
          calcDeuda();
          stockCamello();
          PRECIOS(bar);
          diaSemana();
          checkSecreta();
          BARRIO();
        }
          CLS();
          VPRINT(0,0,"LE PAGAS GUSTOSO");
          WAIT(100);
          dinero = dinero - rf;
          dia = dia -1;
          calcDeuda();
          stockCamello();
          PRECIOS(bar);
          diaSemana();
          checkSecreta();
          BARRIO();      
      }
    if (button < 0 && CURSOR == 1)
      {
      rebound();
      PUTSPRITE(0, 260, 240, 8, 2);
      CLS();
      VPRINT(0,0,"3 EN CARCEL, DROGAS REQUISADAS");
      WAIT(200);
      calcDeuda();
      stockCamello();
      dia = dia - 3;
      int d;
      for (d=0;d<8;++d)
        {
        stock[d] = 0;
        }
      compras = municion;
      PRECIOS(bar);
      diaSemana();
      checkSecreta();
      BARRIO();
      }
  }
}

void hospital() {

  checkFin();

  int CURSOR = 0;

  gui();
  printDatos();

  VPRINT (6,22,"VIAJAR");
  VPRINT(6,6,"CURAR 10 DE VIDA   30");
  VPRINT(6,7,"CURAR 100 DE VIDA 250");
  VPRINT(6,8,"BOTIQUIN PORTATIL 500");

  while(1)
    {
    HALT;
    PlayAY();

    dir = STICK(CURSORKEYS);
    button=STRIG(KEYBOARD_BUTTON);
    animaCursor();
    PUTSPRITE(0, 40+curMovX, posYHOSPI[CURSOR], 8, 2);
    
    if (dir == 1){
      CURSOR = CURSOR - 1;
        if (CURSOR < 0){
          CURSOR=0;
        }
        WAIT(10);
    }
    if (dir == 5)
    {
      CURSOR = CURSOR + 1;
        if (CURSOR > 3)
        {
          CURSOR=3;
        }
        WAIT(10);
    }
    if (button < 0 && CURSOR == 0 && vida < 100 && dinero > 30)
    {
      vida = vida + 10;
      dinero = dinero - 30;
      VPRINTNUMBER (11,2, 5, dinero);
      VPRINTNUMBER(9,3,3, vida);
      WAIT(10);
    }
    if (button < 0 && CURSOR == 1 && vida < 100 && dinero > 250)
    {
      dinero = dinero - 250;
      vida = 100;
      VPRINTNUMBER (11,2, 5, dinero);
      VPRINTNUMBER(9,3,3, vida);
      WAIT(10);
    }
    if (button < 0 && CURSOR == 2 && botiquin == false && dinero > 500)
    {
      dinero = dinero - 500;
      botiquin = true;
      VPRINTNUMBER (11,2, 5, dinero);
    }
    if (button < 0 && CURSOR == 3)
    {
      rebound();
      PUTSPRITE(0, 260, 240, 8, 2);
      VIAJAR();
    }
  Player_Decode();
  }
}

void prestamista() {

  checkFin();

  int CURSOR = 0;
  gui();
  printDatos();
  
  VPRINT (6,22,"VIAJAR");
  VPRINT(6,6,"PAGAR / PEDIR DEUDA");
  VPRINT(6,7,"COMPRAR PISTOLA     500");
  VPRINT(6,8,"TELEFONO DE SOPLON  500");
  VPRINT(6,9,"MOCHILA             500");
  VPRINT(6,10,"COMPRAR MUNICION     50");

  while(1)
    {
    HALT;
    PlayAY();

    dir = STICK(CURSORKEYS);
    button=STRIG(KEYBOARD_BUTTON);

    animaCursor();
    PUTSPRITE(0, 40+curMovX, posYpresta[CURSOR], 8, 2);

    if (dir == 3 && CURSOR == 0)
    {
      deuda = deuda + 100;
      dinero = dinero + 100;
      VPRINTNUMBER(25,2,5, deuda);
      VPRINTNUMBER (11,2, 5, dinero);
      WAIT(10);
    }
    if (dir == 7 && CURSOR == 0 && dinero > 100)
    {
      if (deuda < 100){
        dinero = dinero - deuda;
        deuda = 0;
      }
      if (deuda > 100)
      {
        dinero = dinero -100;
        deuda = deuda - 100;
      }
      VPRINTNUMBER(25,2,5, deuda);
      VPRINTNUMBER (11,2, 5, dinero);
      WAIT(10);
    }
    
    if (dir == 1){
      CURSOR = CURSOR - 1;
        if (CURSOR < 0){
          CURSOR=0;
        }
        WAIT(10);
    }
    if (dir == 5)
    {
      CURSOR = CURSOR + 1;
        if (CURSOR > 5)
        {
          CURSOR=5;
        }
        WAIT(10);
    }

    if (button < 0 && CURSOR == 1 && pistola == false && dinero >= 500)
    {
      dinero = dinero - 500;
      pistola = true;
      VPRINTNUMBER(11,2, 5, dinero);
      WAIT(10);
    }

    if (button < 0 && CURSOR == 2 && soplon == false && dinero >= 500)
    {
      dinero = dinero - 500;
      soplon = true;
      VPRINTNUMBER(11,2, 5, dinero);
      WAIT(10);
    }

    if (button < 0 && CURSOR == 3 && mochila == false && dinero >= 500)
    {
      limiteStock = 50;
      dinero = dinero - 500;
      mochila = true;
      VPRINTNUMBER(11,2, 5, dinero);
      WAIT(10);
    }

    if (button < 0 && CURSOR == 4 && dinero >= 50 && compras < limiteStock)
    {
      compras = compras +1;
      dinero = dinero - 50;
      municion = municion + 1;
      VPRINTNUMBER(11,2, 5, dinero);
      WAIT(10);
    }
    
    if (button < 0 && CURSOR == 5)
    {
    rebound();
    PUTSPRITE(0, 260, 240, 8, 2);
    VIAJAR();
    }
  Player_Decode();
  }
}

void printCAMEL()
{
  int c;
  for (c = 0; c < 8; ++c)
  {
  VPRINTNUMBER(15,6+c,2,stock[c]);
  }
  for (c = 0; c < 8; ++c)
  {
  VPRINTNUMBER(18,6+c,2,camello[c]);
  }
}

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

void WAIT(uint cicles)
{
  uint i;
  for (i=0;i<=cicles;++i)
    {
    HALT;
    PlayAY();
    Player_Decode();
    }
  
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
return address;
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

void VPOKEARRAY(uint vaddr, char* text)
{
  while(*(text)) VPOKE(vaddr++,*(text++));
}

void VPRINTNUMBER(char column, char line, char pLength, uint pNumber)
{
  uint vaddr = BASE10 + (line*32) + column // calcula la posicion en la VRAM...
                 + pLength - 1; // ...empezando por el d??gito m??s a la derecha
  char length = pLength;
  uint number = pNumber;
  
  // Mientras queden d??gitos por pintar...
  while (length) {
  
    // Pinta un d??gito
    uint digit = number % 10; // Calcula el ??ltimo digito (resto de la divisi??n entre 10)
    char digitAscii = '0' + ((char) digit); // Convierte el d??gito en su c??digo ASCII de n??mero
    VPOKE(vaddr, digitAscii); // Pinta el d??gito
    
    // Se mueve al siguiente d??gito
    length--; // Descuenta el d??gito que acabamos de pintar
    vaddr--; // El siguiente d??gito se pintar?? un caracter m??s hacia la izquierda
    number = number / 10; // Divide entre 10 para "avanzar" en el n??mero
 }
}

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

int ran100() {
  
  int a;
  int b = getR();
  a = ((b*100)/128);
  return a;
}

void calcDeuda() {
  int t;
  t = ((deuda / 100)*deudaCiudad);
  deuda = deuda + t;
}

void stockCamello() {
  int a;
        for (a=0;a<8;++a)
        {
        int b;
        int c;
        c = (ran100()/10);
        b = ran100();
        camello[a] = (b / camelloVAR[a]) + c;
        }
}

void overflow(){
int r = (ran100()/50);
int a;
for (a=0;a<r;++a){
}
int o;
o = ((ran100()*8)/100);
precioBARRIO[o] = precioBARRIO[o]/2;
CLS();
VPRINT(0,0,"DROGA: ");
VPRINT(7,0,drugs[o]);
VPRINTNUMBER(0,1,5,precioBARRIO[o]);
WAIT(50);
}

void underflow(){
int r = (ran100()/50);
int a;
for (a=0;a<r;++a){  
}
int o;
o = ((ran100()*8)/100);
precioBARRIO[o] = precioBARRIO[o]*2;
CLS();
VPRINT(0,0,"DROGA: ");
VPRINT(7,0,drugs[o]);
VPRINTNUMBER(0,1,5,precioBARRIO[o]);
WAIT(50);
}

void diaSemana(){
  fecha = fecha + 1;
  if (fecha > 6){
    fecha = 0;
  }
}

void rastro() {
  int r = (ran100()/50);
  int a;
  for (a=0;a<r;++a){  
  }
  int rd = ((ran100()*6)/100); // droga del rastro
  int rc = ((ran100()*10)/100); // cantidad de unidades
  int pdr = ((disp[bar][rd])/3); // precio de la droga en el rastro
  int rdc = 0; //precio total a pagar
  int cr = 0; // las unidades que compramos
  if (rc == 0)
  {
    rc = 1;
  }
  int CURSOR = 0;
  gui();
  printDatos();
  VPRINT (6,22,"VIAJAR");

  VPRINT(6,6,"NAVAJA OXIDADA      100");
  VPRINT(6,7,"CADENAS DE MOTO     200");
  VPRINT(6,8,drugs[rd]);
  VPRINTNUMBER (20,8,1,rc);
  VPRINTNUMBER (24,8,5,pdr);

  
  while(1)
    {
    HALT;
    PlayAY();
    dir = STICK(CURSORKEYS);
    button=STRIG(KEYBOARD_BUTTON);

    animaCursor();
    PUTSPRITE(0, 40+curMovX, posYrastro[CURSOR], 8, 2);
    
    if (dir == 1){
      CURSOR = CURSOR - 1;
        if (CURSOR < 0){
          CURSOR=0;
        }
        WAIT(10);
    }
    if (dir == 5)
    {
      CURSOR = CURSOR + 1;
        if (CURSOR > 3)
        {
          CURSOR=3;
        }
        WAIT(10);
    }

    if (button < 0 && CURSOR == 0 && dinero > 100 && navaja == false)
    {
      dinero = dinero - 100;
      navaja = true;
      VPRINTNUMBER (11,2, 5, dinero);
      WAIT(10);
    }

    if (button < 0 && CURSOR == 1 && dinero > 200 && cadena == false)
    {
      dinero = dinero - 200;
      cadena = true;
      VPRINTNUMBER (11,2, 5, dinero);
      WAIT(10);
    }
    
    if (dir == 7 && CURSOR == 2 && dinero > pdr && rc > 0 && compras < limiteStock)
    {
      compras = compras +1;
      dinero = dinero - pdr;
      stock[rd] = stock[rd] + 1;
      rc = rc - 1;
      cr = cr + 1;
      VPRINTNUMBER (20,8,1,rc);
      VPRINTNUMBER (11,2, 5, dinero);
      rdc = rdc + 1;
      WAIT(10);
    }
    if (button < 0 && CURSOR == 3)
    {
    rebound();
    CLS();
      int ranCalidad;
      int ranCC;
      ranCalidad = ran100();
      if (ranCalidad > 60 && cr > 3)
      {
        VPRINT(0,0,"DEL ");
        VPRINT(5,0,drugs[rd]);
        VPRINT(14,0," QUE HAS COMPRADO");
        VPRINT(0,1,"TE HAN TANGADO");
        ranCC = ((ran100()*4)/100);
        if (ranCC == 0) {
          ranCC =1;
        }
        VPRINTNUMBER(15,1,1,ranCC);
        VPRINT (0,3,"ESTABA CORTADA CON HARINA");
        stock[rd] = stock[rd] - ranCC;
        compras = compras - ranCC;
        WAIT(200);
        VIAJAR();
      } 
      else 
      {
        VIAJAR();
      }
    } 
  Player_Decode();
  }
}

void pijo() {
  int x = 0;
  CLS();
  int CURSOR = 0;
  int cur = 0;
  int pijoDroga = ((ran100()*7)/100);
  VPRINT(0,0,"PIJO CON MONAZO, QUIERE:");
  VPRINT(7,1,drugs[pijoDroga]);
  VPRINT(1,4,"SI");
  VPRINT(4,4,"NO");
  VPRINT(7,4,"HOSTIA Y PALANTE");
  PUTSPRITE(0, cur,32, 8, 2);
  while(1)
    {
    HALT;
    dir = STICK(CURSORKEYS);
    button=STRIG(KEYBOARD_BUTTON);
    if (dir == 3)
    {
      CURSOR = CURSOR + 1;
      cur = cur +24;
        if (CURSOR > 2)
        {
          CURSOR=2;
          cur = 48;
        }
        WAIT(10);
        PUTSPRITE(0, cur, 32, 8, 2);
    }
    if (dir == 7)
    {
      CURSOR = CURSOR - 1;
      cur=cur-24;
        if (CURSOR < 0)
        {
          CURSOR=0;
          cur = 0;
        }
        WAIT(10);
        PUTSPRITE(0, cur, 32, 8, 2);
    }
    if (button < 0 && CURSOR == 0)
      {
      PUTSPRITE(0, 260, 240, 8, 2);
      if (stock[pijoDroga] > 0) 
        {
        CLS();
        
        int cantidad = ((ran100()*4)/100);
        
        if (cantidad == 0)
        {
          cantidad = 1;
        }
        if (stock[pijoDroga] < cantidad){
          cantidad = stock[pijoDroga];
        }
        int precioPijo;
        VPRINT(0,4,"TIENES ");
        VPRINTNUMBER(8,4,2,stock[pijoDroga]);
        VPRINT(0,5,"LE VENDES");
        VPRINTNUMBER(10,5,1,cantidad);
        WAIT(50);
        precioPijo = (precioBARRIO[pijoDroga] * 3) * cantidad;
        dinero = dinero + precioPijo;
        stock[pijoDroga] = stock[pijoDroga] - cantidad;
        compras = compras - cantidad;
        VPRINT(0,6,"SE LA VENDES POR ");
        VPRINTNUMBER(17,6,5,precioPijo);
        dia = dia -1;
        calcDeuda();
        stockCamello();
        WAIT(200);
        PRECIOS(bar);
        diaSemana();
        checkSecreta();
        BARRIO();
        }
      else 
        {
        CLS();
        VPRINT(0,0,"NO TIENES,TE INTENTA PEGAR");
        WAIT(100);
        peleaBarra(0);
        }
      }
    if (button < 0 && CURSOR == 1)
      {
      PUTSPRITE(0, 260, 240, 8, 2);
      CLS();
      VPRINT(0,0,"NO VENDES, TE ATACA");
      WAIT(100);
      peleaBarra(0);
      }
    if (button < 0 && CURSOR == 2)
      {
      PUTSPRITE(0, 260, 240, 8, 2);
      CLS();
      VPRINT(0,0,"LE SUELTAS UNA HOSTIA"); 
      WAIT(50);
      peleaBarra(0);
      }
  }
}

void checkSecreta(){
   int randomSecreta;
  randomSecreta = (ran100()/10);
  if (randomSecreta > 8 && secretaPosible == true){
    haySecreta = true;
  }
}

void imprimeBarrio(int barrio,int barrioAhora) {
  
  switch (barrioAhora)
  {
  case 1:
    VPRINT (11,1,bilbao[barrio]);
    break;
  case 2:
    VPRINT (11,1,madrid[barrio]);
    break;
  case 3:
    VPRINT (11,1,newyork[barrio]);
    break;
  default:
    break;
  }
}

// void imprimeIcono(char column, char line, char caracter){
//   int x = 0;
//   int y = 0;
//   char c = 0;
//   uint vaddr = BASE10 + (line*32) + column; 
  
//   for (x=0; x<2; ++x)
//   {
//     for (y=0; y<2; ++y)
//     {
//     VPOKE(vaddr+(x*32)+(y+32),caracter+c);
//     c = c +1;
//     } 
//   }
//}

void peleaBarra(int tipo)
{
  boolean victoriaBarra = false;
  int velocidad = 0;
  int vidaRestar = 0;
  int izd = 64;
  int drch = 74;

  if (navaja == true)
  {
    izd = izd - 8;
    drch = drch + 8;
    VPRINT(6, 10, "XXXXXXXOOOOXXXXXXX");
  }
  else
  {
    VPRINT(6, 10, "XXXXXXXXOOXXXXXXXX");
  }

  if (tipo == 0)
  {
    velocidad = 4;
    vidaRestar = 20;
  }
  if (tipo == 1)
  {
    velocidad = 3;
    vidaRestar = 50;
  }
  if (tipo == 2)
  {
    velocidad = 5;
    vidaRestar = 60;
  }
  if (cadena == true)
  {
    velocidad = velocidad - 1;
  }
  boolean subiendo = true;

  PUTSPRITE(0, 80, 88, 3, 0);
  int x = 0;
  
  while (1)
  {
    HALT;
    PlayAY();
    if (subiendo == true)
    {

      x = x + velocidad;
      PUTSPRITE(0, x + 48, 88, 3, 0);
      button = STRIG(KEYBOARD_BUTTON);

      if (button == -1)
      {
        if (x < izd || x > drch)
        {
          victoriaBarra = false;
          resultadoBarra(victoriaBarra, tipo, vidaRestar);
        }
        else
        {
          victoriaBarra = true;
          resultadoBarra(victoriaBarra, tipo, vidaRestar);
        }
      }
      if (x > 140)
      {
        subiendo = false;
      }
    }
    else
    {
      x = x - velocidad;
      PUTSPRITE(0, x + 48, 88, 3, 0);
      button = STRIG(KEYBOARD_BUTTON);
      
      if (button == -1)
      {
        if (x < izd || x > drch)
        {
          victoriaBarra = false;
          resultadoBarra(victoriaBarra, tipo, vidaRestar);
        }
        else
        {
          victoriaBarra = true;
          resultadoBarra(victoriaBarra, tipo, vidaRestar);
        }
      }
      if (x <= 0)
      {
        subiendo = true;
      }
    }
    Player_Decode();
  }
}

void resultadoBarra(boolean victoria, int personaje, int vidaMenos)
{
  CLS();
  if (victoria == false)
  {
    CLS();
    if (personaje == 0)
    {
      VPRINT(0, 0, "EL PIJO TE ARREA");
    }
    if (personaje == 1)
    {
      VPRINT(0, 0, "EL JONKI TE PEGA MOJADA");
    }
    if (personaje == 2)
    {
      VPRINT(0, 0, "EL POLI TE PEGA UN TIRO");
    }
    WAIT(200);
    dia = dia - 1;
    vida = vida - vidaMenos;
    PRECIOS(bar);
    calcDeuda();
    stockCamello();
    diaSemana();
    checkSecreta();
    BARRIO();
  }
  else
  {
    CLS();
    if (personaje == 0)
    {
      VPRINT(0, 0, "LE PEGAS UNA HOSTIA AL PIJO");
      r = (ran100() * 4);
      VPRINT(0, 1, "LE ROBAS ");
      VPRINTNUMBER(10, 1, 4, r);
      dinero = dinero + r;
    }
    if (personaje == 1)
    {
      VPRINT(0, 0, "LE ZUMBAS AL JONKI");
      ganasJonki();
    }
    if (personaje == 2)
    {
      VPRINT(0, 0, "LE DAS UNA PALIZA AL POLI");
    }
    WAIT(200);
    dia = dia - 1;
    calcDeuda();
    stockCamello();
    WAIT(200);
    PRECIOS(bar);
    diaSemana();
    checkSecreta();
    BARRIO();
  }
}

void intentarEscapar(int tipo){
  PUTSPRITE(0, 260, 240, 8, 2);
  int velocidad = 0;
  int diasRestar = 0;
  int vidaRestar = 0;
  if (tipo == 0){
  velocidad = 14;
  diasRestar = 1;
  vidaRestar = 50;
  }
  if (tipo == 1){
  velocidad = 11;
  diasRestar = 3;
  vidaRestar = 50;
  }
  
  CLS();
  VPRINT(0,0,"CORRE FORREST!");
  if (tipo == 0){
  VPRINT(0,2,"EL JONKI");
  }
  if (tipo == 1){
  VPRINT(0,2,"EL POLI");
  }
  int total = 0;
  int medio = 0;
  int jonka = 0;
  int jonkb = 0;
  WAIT(50);
  VPRINT(0,5,"3");
  WAIT(50);
  VPRINT(0,5,"2");
  WAIT(50);
  VPRINT(0,5,"1");
  WAIT(50);
  VPRINT(0,5,"0!");
  while(1)
  {
  dir = STICK(CURSORKEYS);
    if (dir == 3 && medio == 0)
    {
      medio = medio + 1;
      WAIT(3);
    }
    if (dir == 7 && medio == 1)
    {
      medio = medio + 1;
      WAIT(3);
    }
    if (dir == 7 && medio == 0)
    {
      WAIT(3);
    }
    if (medio == 2)
    {
      total = total + 1;
      medio = 0;
      VPRINTNUMBER(0,1,2,total);
      WAIT(3);
    }
    jonka = jonka + 1;
    if (jonka > velocidad)
    {
      jonkb = jonkb +1;
      jonka = 0;
    }
    if (total > 99){
      CLS();
      VPRINT(0,0,"CONSIGUES ESCAPAR");
      WAIT(100);
      dia = dia -1;
      calcDeuda();
      stockCamello();
      WAIT(200);
      PRECIOS(bar);
      diaSemana();
      checkSecreta();
      BARRIO(); 
    }
    if (jonkb > 99){
      CLS();
      VPRINT(0,0,"TE PILLO!");
      WAIT(100);
      vida = vida - vidaRestar;
      dia = dia - diasRestar;
      calcDeuda();
      stockCamello();
      WAIT(200);
      PRECIOS(bar);
      diaSemana();
      checkSecreta();
      BARRIO(); 
    }
    VPRINTNUMBER(0,3,2,jonkb);
  }
}

void disparos(){

  CLS();
  int posicionPolicia[] = {0,10,7,15,14,12,20,7};
  int curX = 128;
  int curY = 0;
  int polX = 0;
  int polY = 0;
  int sitioX = 0;
  int sitioY = 0;
  int posicion = (ran100()*4)/100;



sitioX = posicionPolicia[0] + posicion;
  sitioY = posicionPolicia[1] + posicion;
  VPRINT(sitioX,sitioY,"OOOO");
  VPRINT(sitioX,sitioY+1,"OOOO");
  VPRINT(sitioX,sitioY+2,"OOOO");
  VPRINT(sitioX,sitioY+3,"OOOO");
 
  PUTSPRITE(0, curX, curY, 5, 1);
  

  int tiempo = 50;
  int segundo = 1;

  VPRINTNUMBER(0,0,1,segundo);
  
  while(1)
    {
      VPRINT(1,0,":");
      VPRINTNUMBER(2,0,2,tiempo);
      tiempo = tiempo - 1;
      if (tiempo < 0) { 
        tiempo = 50;
        segundo = segundo - 1;
        VPRINTNUMBER(0,0,1,segundo);
      }
      if (segundo < 0)
        {
        CLS();
        VPRINT(0,7,"EL POLI TE PEGA UN TIRO");
        PUTSPRITE(0, 250, 250, 5, 1);
        WAIT(200);
        dia = dia -1;
        vida = vida - 60;
        PRECIOS(bar);
        calcDeuda();
        stockCamello();
        diaSemana();
        checkSecreta();
        BARRIO(); 
      } 
    button = button=STRIG(KEYBOARD_BUTTON);
    dir = STICK(CURSORKEYS);
    if (dir == 1 && curY > 0)
    {
      curY=curY -8;
    }
    if (dir == 2 && curY > 0 && curX < 200)
    {
      curY=curY -8;
      curX=curX +8;
    }
    if (dir == 3 && curX < 200)
    {
      curX=curX +8;
    }
    if (dir == 4 && curY >= 0 && curX < 200 && curY < 180)
    {
      curY=curY +8;
      curX=curX +8;
    }
    if (dir == 5 && curY < 180)
    {
      curY=curY +8;
    }
    if (dir == 6 && curY >= 0 && curX > 0 && curY < 180)
    {
      curY=curY +8;
      curX=curX -8;
    }
    if (dir == 7 && curX > 0)
    {
      curX=curX -8;
    }
    if (dir == 8 && curY > 0 && curX > 0)
    {
      curY=curY -8;
      curX=curX -8;
    }
    if (button == -1) {
        if (curX >= (sitioX*8) && curX <= ((sitioX*8)+26) && curY >= (sitioY*8) && curY <= ((sitioY*8)+26)) 
        {
        CLS();
        VPRINT(0,6,"DENTRO, CONSIGUES ESCAPAR");
        PUTSPRITE(0, 250, 250, 5, 1);
        WAIT(200);
        municion = municion - 1;
        compras = compras - 1;
        dia = dia -1;
        calcDeuda();
        stockCamello();
        WAIT(200);
        PRECIOS(bar);
        diaSemana();
        checkSecreta();
        BARRIO(); 
        }
        else
        {
        VPRINT(0, 6, "FUERA, TE PEGA UN TIRO");
        PUTSPRITE(0, 250, 250, 5, 1);
        WAIT(200);
        compras = compras - 1;
        municion = municion - 1;
        dia = dia -1;
        vida = vida - 60;
        PRECIOS(bar);
        calcDeuda();
        stockCamello();
        diaSemana();
        checkSecreta();
        BARRIO();  
        }
    }
    PUTSPRITE(0,curX,curY,5,1);
    WAIT(1);
  }
}

void ganasJonki()
{
r = ((ran100()*4)/100);
            if (r<1)
              { 
              r = 1;
              }
            int rr = ((ran100()*7)/100);
            int d = ran100();
            VPRINT(0,1,"EL JONKI LLEVA ");
            VPRINTNUMBER(15,1,1,r);
            VPRINT(17,1,"DE ");
            VPRINT(21,1,drugs[rr]);
            VPRINT(0,2,"Y ");
            VPRINTNUMBER(2,2,3,d);
            VPRINT(6,2,"DE DINERO");
            int limite = limiteStock - compras;
            if ((compras + r) >= limiteStock)
              {
              VPRINT(0,3,"SOBREPASAS STOCK");
              VPRINT(0,4,"PUEDES PILLAR");
              VPRINTNUMBER(28,4,1,limite);
              compras = compras + limite;
              dinero = dinero + d;
              stock[rr] = stock[rr] + limite;
              }
            else 
              {
              dinero = dinero + d;
              stock[rr] = stock[rr] + r;
              compras = compras + r;
              }
      dia = dia -1;
      calcDeuda();
      stockCamello();
      WAIT(200);
      PRECIOS(bar);
      diaSemana();
      checkSecreta();
      BARRIO();
}

void rebound()
{
  button=STRIG(KEYBOARD_BUTTON);
  while (button == -1)
  {
  button=STRIG(KEYBOARD_BUTTON);
  WAIT(1);
  }
}

void checkFin()
{
 if (vida<10){
    CLS();
    VPRINT(0,0,"GAMEOVER");
    WAIT(500);
    inicializar();
  }
  if (dia <= 0 && deuda > 0){
    CLS();
    VPRINT(0,0,"GAMEOVER");
    WAIT(500);
    inicializar();
  }
  if (vida <= 0 && botiquin == true){
    CLS();
    VPRINT(0,0,"SIN VIDA, USAS BOTIQUIN");
    vida = 40;
    botiquin = false;
    WAIT(200);
    BARRIO();
  }
  if (dia == 0 && deuda == 0){
    CLS();
    VPRINT(0,0,"GENIAL! HAS PAGADO LA DEUDA");
    VPRINT(0,1," Y UNOS BENEFICIOS DE");
    VPRINTNUMBER(23,1,5,dinero);
    WAIT(500);
    inicializar();
  }
  
}

void printDatos()
{
  VPRINT (3,1, "Barrio: ");
  imprimeBarrio(bar,barrioPartida);
  VPRINT (19,1, "Dia: ");
  VPRINTNUMBER(24,1,3, dia);
  VPRINT (3,2, "Dinero: ");
  VPRINTNUMBER (11,2, 5, dinero);
  VPRINT (19,2, "Deuda: ");
  VPRINTNUMBER(25,2,5, deuda);
  VPRINT (3,3, "Vida: ");
  VPRINTNUMBER(9,3,3, vida);
}

void animaCursor()
{
 if (subiendo == true)
  {
  if (curMovX < 3)
    {
    ++curMovX;
    subiendo = true;
    }
  else
    {
      subiendo = false;
    }
  }
  if (subiendo == false)
  {
    if (curMovX > -3){
      --curMovX;
      subiendo = false;
    }
    else
    {
      subiendo = true;
    }
  }
}