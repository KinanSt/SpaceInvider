#ifndef BULLET_H
#define BULLER_H

#include <stdint.h>

typedef struct Bullet {
  uint8_t posX;
  uint8_t posY;

  int lastMoveTime;
} Bullet;

#endif