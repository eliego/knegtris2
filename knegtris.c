#include "knegtris.h"
#include "kloss.h"
#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main() {
  struct timespec t;
  t.tv_sec = 0;
  t.tv_nsec = 200000000;
  initGUI();
  srand(time(NULL));
  struct kloss Kloss;
  
  while ((Kloss.posY >= 1) && (initKloss(&Kloss), ritaKloss(&Kloss, 1) != -1)) {
    wrefresh(spelFonster);
    while (nanosleep(&t, NULL), 1) {
      ritaKloss(&Kloss, 0);
      Kloss.posY++;
      if (ritaKloss(&Kloss, 1) == -1) {
	Kloss.posY--;
	ritaKloss(&Kloss, 1);
	break;
      }
      wrefresh(spelFonster);
    }      
  }

  endwin();
  return 0;
}

void initKloss(struct kloss* Kloss) {
  int x = 4;
  Kloss->giltig = -1; 
  Kloss->typ = rand() % 7; 
  Kloss->vinkel = rand() % 4; 

  switch (Kloss->typ) {
  case PINNE:
    x = 0;
    if (Kloss->vinkel == OST || Kloss->vinkel == VAST)
      Kloss->posY = 1;
    else
      Kloss->posY = -2;
    break;

  case Z1:
  case Z2:
    if (Kloss->vinkel == OST || Kloss->vinkel == VAST)
      Kloss->posY = 0;
    else
      Kloss->posY = -1;
    break;

  case L1:
  case L2:
    if (Kloss->vinkel == OST || Kloss->vinkel == VAST)
      Kloss->posY = 0;
    else
      Kloss->posY = -1;
    break;

  case FYRKANT:
    Kloss->posY = 0;
    break;

  case T:
    if (Kloss->vinkel == OST)
      Kloss->posY = -1;
    if (Kloss->vinkel == VAST)
      Kloss->posY = 0;
    else
      Kloss->posY = -1;      
  }

  Kloss->posX = (STORLEK_X - 6) / 2 + 1; 
  return;
}

void snurraPunkt(int* y, int* x, enum vaderstreck vinkel, enum klossTyp typ) {
    int temp, y0, x0;

  if (typ == PINNE || typ == Z1 || typ == Z2) {
    if (vinkel == SYD)
      vinkel = NORD;
    if (vinkel == VAST)
      vinkel = OST;
  }

  if (typ == FYRKANT)
    return;

  switch (typ) {
  case PINNE:
    y0 = 1;
    x0 = 1;
    break;

  case Z1:
  case Z2:
    y0 = 1;
    x0 = 1;
    break;

  case L1:
    y0 = 1;
    x0 = 1;
    break;

  case L2:
    y0 = 1;
    x0 = 0;
    break;

  case T:
    y0 = 1;
    x0 = 0;
  }

  switch (vinkel) {
  case NORD:
    return;

  case OST:
    temp = *x;
    *x = -(*y - y0) + x0;
    *y = temp - x0 + y0;
    break;
    
  case SYD:
    *x = -*x + 2*x0;
    *y = -*y + 2*y0;
    break;

  case VAST:
    temp = *x;
    *x = *y - y0 + x0;
    *y = -(temp - x0) + y0;
  }
  
  return;
}

void initGUI() {
  initscr();
  curs_set(0);
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  refresh();

  spelFonster = newwin(STORLEK_Y, STORLEK_X, (LINES - STORLEK_Y)/2, (COLS - STORLEK_X)/2);
  wborder(spelFonster, '|', '|', '-', '-', '+', '+', '+', '+');
  refresh();
  wrefresh(spelFonster);
}

int* getData(enum klossTyp typ) {
  static int data[8];
  static enum klossTyp forraTyp = 10;

  if (typ == forraTyp)
    return data;

  forraTyp = typ;

  int a,b,p = 0, q = 1;
  for (a = 0; a < 4; a++)
    for (b = 0; b < 2; b++)
      data[a*2+b] = 0;

  switch (typ) {
  case PINNE:
    for (a = 0; a < 4; a++)
      data[a*2] = 1;
    break;

  case Z1:
    p = 1, q = 0;

  case Z2:
    for (a = 0; a < 2; a++)
      data[a*2+p] = 1;
    for (a = 1; a < 3; a++)
      data[a*2+q] = 1;
    break;
  
  case L1:
    p = 1, q = 0;
  
  case L2:
    for (a = 0; a < 3; a++)
      data[a*2+p] = 1;
    data[4+q] = 1;
    break;
    
  case FYRKANT:
    for (a = 0; a < 2; a++)
      for (b = 0; b < 2; b++)
	data[a*2+b] = 1;
    break;

  case T:
    for (a = 0; a < 3; a++)
      data[a*2] = 1;
    data[3] = 1;
  }

  return data;
}  

int ritaKloss(struct kloss* kloss, int rita) {
  int a,b, a2, b2;
  int* data = getData(kloss->typ);

  if (rita > 0) {
    for (a = 0; a < 4; a++)
      for (b = 0; b < 2; b++)
	{
	  if (data[a*2+b] < 1)
	    continue;
	  
	  a2 = a, b2 = b;
	  snurraPunkt(&a2, &b2, kloss->vinkel, kloss->typ);	
	  
	  if (kloss->posY + a2 >= STORLEK_Y - 1) {
	    kloss->giltig = -1;
	    return -1;
	  }
	  if (kloss->posX + b2 < 1 || kloss->posX + 2 * b2 >= STORLEK_X - 1) {
	    kloss->giltig = -1;
	    return -1;
	  }
	  
	  if ((char)(mvwinch(spelFonster, kloss->posY + a2, kloss->posX + 2 * b2) & A_CHARTEXT) == '[') {
	    kloss->giltig = -1;
	    return -1;
	  }
	}
  } else
    if (kloss->giltig < 0)
      return -1;

  
  for (a = 0; a < 4; a++)
    for (b = 0; b < 2; b++) {
	  if (data[a*2+b] < 1)
	    continue;

	  a2 = a, b2 = b;
	  snurraPunkt(&a2, &b2, kloss->vinkel, kloss->typ);

	  if (kloss->posY + a2 < 1)
	    continue;

	  if (rita > 0) {
	    wmove(spelFonster, kloss->posY + a2, kloss->posX + 2 * b2);
	    waddch(spelFonster, '[');
	    waddch(spelFonster, ']');
	  } else {
	    wmove(spelFonster, kloss->posY + a2, kloss->posX + 2 * b2);
	    wdelch(spelFonster);
	    wdelch(spelFonster);
	    winsch(spelFonster, ' ');
	    winsch(spelFonster, ' ');
	  }  
    }

  if (kloss->giltig < 0)
    kloss->giltig = 0;

  return 0;
      
}
