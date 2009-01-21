#include <curses.h>
#include "kloss.h"

#ifndef _KNEGTRIS_H_
#define _KNEGTRIS_H_

#define STORLEK_X 22
#define STORLEK_Y 40

static WINDOW* spelFonster;
static WINDOW* poangFonster;
static WINDOW* nastaKlossFonster;

void initGUI(void);
void initKloss(struct kloss*);
int* getData(enum klossTyp);
void snurraPunkt(int*, int*, enum vaderstreck, enum klossTyp);
int ritaKloss(struct kloss*, int);

#endif
