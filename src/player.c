#include "../include/player.h"


void playerMoveLeft(Player* player, int min, uint16_t distance) {
  if (player->posX >= min+distance) {
    player->posX -= distance;
  } else {
    player->posX = min;
  }
}

void playerMoveRight(Player* player, int max, uint16_t distance) {
  if (player->posX <= max-distance) {
    player->posX += distance;
  } else {
    player->posX = max;
  }
}