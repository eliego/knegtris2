#ifndef _KNEGTRIS_KLOSS_
#define _KNEGTRIS_KLOSS_

enum klossTyp {
  PINNE,
  Z1,
  Z2,
  L1,
  L2,
  FYRKANT,
  T
};

enum vaderstreck {
  NORD,
  OST,
  SYD,
  VAST
};


struct kloss {
  enum klossTyp typ;
  int posX;
  int posY;
  enum vaderstreck vinkel;
  int giltig;
  int hojd;
};

#endif
