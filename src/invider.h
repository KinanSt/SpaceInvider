#ifndef INVIDER_H
#define INVIDER_H

#include <stdint.h>

typedef struct Invider {
  uint8_t level;
  int8_t health;
  uint8_t posX;
  uint8_t posY;
  
  int lastMoveTime;
} Invider;



#endif