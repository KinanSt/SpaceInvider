#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>

typedef struct Player {
  uint8_t health;
  uint8_t posX;
  uint8_t posY;
  uint16_t score;
  
  int lastShotMs;
} Player;

void playerMoveLeft(Player* player, int min, uint16_t distance);

void playerMoveRight(Player* player, int max, uint16_t distance);


#endif