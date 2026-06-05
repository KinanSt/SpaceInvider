/* 
Projet final : Space invider like

Joueur : ^
Invider : $
Dead invier : 0.5s +

Balles *


Plateau : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h> // uint***
#include <time.h>
#include <windows.h>
#include <conio.h>

#include "../libs/list.h"


#define HEIGHT 20
#define WIDTH 20
#define OFFSET_X 5
#define OFFSET_Y 3
#define MAX_FPS 20
#define GAME_BORDERS ' '

#define BULLET_INTERVAL_MS 500
#define BULLET_MOVE_INTERVAL_MS 100

#define INVIDER_MOVE_INTERVAL_MS 500
#define INVIDER_SPAWN_INTERVAL_MS 1000
#define INVIDER_HEALTH 1

#define PLAYER_Y 2
#define PLAYER_HEALTH 5

#define moveTo(x, y) printf("\033[%d;%dH", (y)+1, (x)+1)
#define moveToGame(x, y) printf("\033[%d;%dH", OFFSET_Y + HEIGHT - (y), OFFSET_X + 1 + (x))
#define clear() printf("\033[2J")
#define hideCursor() printf("\033[?25l")
#define showCursor() printf("\033[?25h")

#define red_text() printf("\033[31m")
#define blue_text() printf("\033[34m")
#define white_text() printf("\033[37m")
#define black_text() printf("\033[30m")

#define red_background() printf("\033[41m")
#define blue_background() printf("\033[44m")
#define white_background() printf("\033[47m")
#define black_background() printf("\033[40m")
#define light_gray_background() printf("\033[90m")


#define reset() printf("\033[0m")



struct Invider {
  uint8_t level;
  int8_t health;
  uint8_t posX;
  uint8_t posY;
  
  int lastMoveTime;
};

struct Player {
  uint8_t health;
  uint8_t posX;
  uint8_t posY;
  uint16_t score;
  
  int lastShotMs;
};

struct Game {
  int lastTickMs;
  int lastSpawnMs;
  //boolean moved;
  uint8_t playing; // 0 -> stop ; 1 -> start menu ; 2 -> play ; 3 -> menu end
};

uint8_t lastPlaying = 0;

struct Bullet {
  uint8_t posX;
  uint8_t posY;

  int lastMoveTime;
};

struct ListDoubleChaine inviders;
struct ListDoubleChaine bullets;

struct Player player;
struct Game game;

uint8_t startMenuBtnIndex = 0;

int bestScore = -1;


void clearRect(int x1, int y1, int width, int height) {
  for (int y = 0; y < height; y++) {
    moveTo(x1, y1+y);

    for (int x = 0; x < width; x++) putchar(' ');
  }
}

struct DoubleChaine* deleteInvider(struct DoubleChaine *inviderElement) {
  free(inviderElement->value);

  struct DoubleChaine *nextElement = inviderElement->next;

  supprimer(&inviders, inviderElement);

  return nextElement;
}

struct DoubleChaine* deleteBullet(struct DoubleChaine *bulletElement) {
  free(bulletElement->value);

  struct DoubleChaine *nextElement = bulletElement->next;

  supprimer(&bullets, bulletElement);

  return nextElement;
}

struct DoubleChaine* checkBulletCollision(struct DoubleChaine *bulletElement) {
  struct Bullet *bullet = bulletElement->value;
  struct DoubleChaine *currentElement = inviders.first;

  while (currentElement != NULL) {

    struct Invider *invider = currentElement->value;

    if (invider->posX == bullet->posX && invider->posY == bullet->posY) {
      invider->health--;

      if (invider->health <= 0) {
        player.score++;
        deleteInvider(currentElement);
      }

      // delete bullet and return next element
      return deleteBullet(bulletElement);
        
    }

    currentElement = currentElement->next;
  }

  return bulletElement->next;
}

struct DoubleChaine* checkInviderCollision(struct DoubleChaine *inviderElement) {
  struct Invider *invider = inviderElement->value;
  struct DoubleChaine *currentElement = bullets.first;

  while (currentElement != NULL) {

    struct Bullet *bullet = currentElement->value;

    if (invider->posX == bullet->posX && invider->posY == bullet->posY) {
      invider->health--;

      currentElement = deleteBullet(currentElement);

      if (invider->health <= 0) {
        player.score++;
        return deleteInvider(inviderElement);
      }

      continue; 
        
    }

    currentElement = currentElement->next;
  }

  return inviderElement->next;
}


int getBestScore() {
  if (bestScore != -1) return bestScore;
  
  FILE *f = fopen("best_score", "r");


  if (f != NULL) {
    int result;

    fscanf(f, "%d", &result);

    fclose(f);

    return result;
  } else {;
    return 0;
  }
}

void saveBestScore(int newBestScore) {
  bestScore = newBestScore;

  FILE *f = fopen("best_score", "w");


  if (f != NULL) {

    fprintf(f, "%d", newBestScore);

    fclose(f);
  }
}

void endGame() {
  struct DoubleChaine *currentElement = bullets.first;
  while (currentElement != NULL) currentElement = deleteBullet(currentElement);

  currentElement = inviders.first;
  while (currentElement != NULL) currentElement = deleteInvider(currentElement);

  game.playing = 3;

  if (player.score > getBestScore()) saveBestScore(player.score);
}

void startGame() {
  player.posY = PLAYER_Y;
  player.posX = WIDTH/2;
  player.health = PLAYER_HEALTH;
  player.score = 0;
  player.lastShotMs = GetTickCount();

  game.lastTickMs = 0;
  // game.moved = 0;
  game.playing = 2;
}


void drawGame() {
  // clear game
  //light_gray_background();
  clearRect(OFFSET_X, OFFSET_Y, WIDTH, HEIGHT);



  // draw bullets
  struct DoubleChaine *currentBullet = bullets.first;

  while (currentBullet != NULL) {
    struct Bullet *bullet = currentBullet->value;

    moveToGame(bullet->posX, bullet->posY);
    blue_text();
    putchar('*');

    currentBullet = currentBullet->next;
  }


  // draw inviders
  struct DoubleChaine *currentInvider = inviders.first;

  while (currentInvider != NULL) {
    struct Invider *invider = currentInvider->value;

    moveToGame(invider->posX, invider->posY);
    red_text();
    putchar('$');

    currentInvider = currentInvider->next;
  }

  // draw player
  moveToGame(player.posX, player.posY);
  white_text();
  putchar('^');

  moveToGame(0, -2);
  black_background();
  printf("Score : %d", player.score);

  moveToGame(0, -3);
  printf("Vie : %d/%d", player.health, PLAYER_HEALTH);
}

void drawStartMenu() {
  black_background();
  clearRect(OFFSET_X, OFFSET_Y, WIDTH, HEIGHT);

  moveToGame(WIDTH / 2 - 7, HEIGHT/2);

  if (startMenuBtnIndex == 0) {
    black_text();
    white_background();
  } else {
    white_text();
    black_background();
  }

  printf("start");

  moveToGame(WIDTH / 2 + 2, HEIGHT/2);

  if (startMenuBtnIndex == 1) {
    black_text();
    white_background();
  } else {
    white_text();
    black_background();
  }

  printf("quit");
}

void drawEndMenu() {
  black_background();
  clearRect(OFFSET_X, OFFSET_Y, WIDTH, HEIGHT);

  moveToGame(WIDTH / 2 - 2, HEIGHT/2);

  black_text();
  white_background();

  printf("back");

  black_background();
  white_text();
  moveToGame(WIDTH / 2 - 6, HEIGHT/2 + 4);
  printf("You lose...");

  moveToGame(WIDTH / 2 - (16 + (player.score > 0? (int) floor(log10(player.score)) + 1 : 0))/2, HEIGHT/2 + 3);
  printf("Your score is : %d", player.score);

  int _bestScore = getBestScore();
  moveToGame(WIDTH / 2 - (16 + (_bestScore > 0? (int) floor(log10(_bestScore)) + 1 : 0))/2, HEIGHT/2 + 2);
  printf("Best score is : %d", _bestScore);

}

void drawBox() {
  // Draw box
  black_text();
  black_background();

  for (int offset_y = 0; offset_y < OFFSET_Y; offset_y++) {
    moveTo(0, offset_y);
    for (int i = 0; i < WIDTH+ 2*OFFSET_X ; i++) putchar(GAME_BORDERS);
  }
  
  for (int i = 0; i < HEIGHT; i++) {
    moveTo(0, OFFSET_Y + i);
    for (int offset_x = 0; offset_x < OFFSET_X; offset_x++) putchar(GAME_BORDERS);

    moveTo(OFFSET_X + WIDTH, OFFSET_Y + i);
    for (int offset_x = 0; offset_x < OFFSET_X; offset_x++) putchar(GAME_BORDERS);
  }
  
  for (int offset_y = 0; offset_y < OFFSET_Y; offset_y++) {
    moveTo(0, HEIGHT+OFFSET_Y + offset_y);
    for (int i = 0; i < WIDTH+ 2*OFFSET_X ; i++) putchar(GAME_BORDERS);
  }

  moveTo(0, HEIGHT+2*OFFSET_Y);
  for (int i = 0; i < WIDTH + 2*OFFSET_X ; i++) putchar(GAME_BORDERS);
}

void draw() {
  if (game.playing != lastPlaying) {
    lastPlaying = game.playing;
    drawBox();
  }


  switch (game.playing) {
    case 1:
      drawStartMenu();
      break;

    case 2:
      drawGame();
      break;

    case 3:
      drawEndMenu();
      break;
    
    default:
      drawStartMenu();
      break;
  }

  reset();
  moveToGame(-OFFSET_X, -OFFSET_Y+1);
}

void drawAll() {
  // clear console
  clear();

  drawBox();  

  // Draw game
  draw();

}


void bulletsTick(DWORD actualTime) {
  struct DoubleChaine *currentElement = bullets.first;

  while (currentElement != NULL) {

    struct Bullet *bullet = currentElement->value;

    if (actualTime - bullet->lastMoveTime > BULLET_MOVE_INTERVAL_MS) {
      if (bullet->posY < HEIGHT - 1) {
        bullet->posY++;
        bullet->lastMoveTime = actualTime;

        currentElement = checkBulletCollision(currentElement);
        
        continue;

      } else {

        currentElement = deleteBullet(currentElement);
        continue;

      }

    }

    currentElement = currentElement->next;
  }
}

void invidersTick(DWORD actualTime) {
  if (actualTime - game.lastSpawnMs > INVIDER_SPAWN_INTERVAL_MS) {
    struct Invider *newInvider = malloc(sizeof(struct Invider));
    newInvider->posX = rand() % WIDTH;
    newInvider->posY = HEIGHT;
    newInvider->health = INVIDER_HEALTH;
    newInvider->lastMoveTime = 0;
    ajouter_fin(&inviders, newInvider);

    game.lastSpawnMs = actualTime;
  }

  struct DoubleChaine *currentElement = inviders.first;

  while (currentElement != NULL) {

    struct Invider *invider = currentElement->value;

    if (actualTime - invider->lastMoveTime > INVIDER_MOVE_INTERVAL_MS) {
      if (invider->posY > 0) {
        invider->posY--;
        invider->lastMoveTime = actualTime;

        currentElement = checkInviderCollision(currentElement);
        continue;

      } else {

        player.health--;

        if (player.health <= 0) {
          // lose
          endGame();
          printf("fin");
          return;
        }

        currentElement = deleteInvider(currentElement);
        continue;
      }

    }

    currentElement = currentElement->next;
  }
}

void gameTick(DWORD actualTime) {

  while (_kbhit()) {          // une touche est pressée ?
    char c = _getch();        // lire la touche sans attendre Enter
    
    if (c == 75) { // Fleche gauche
      if (player.posX != 0/* && !game.moved*/) {
        player.posX--;
        //game.moved = 1;
      }
    } else if (c == 77) { // Fleche droite
      if (player.posX < WIDTH-1/* && !game.moved*/) {
        player.posX++;
        //game.moved = 1;
      }
    } else if (c == 'q') {
      game.playing = 0;
      return;
    } else if (c == ' ') {
      struct Bullet *newBullet = malloc(sizeof(struct Bullet));
      newBullet->posX = player.posX;
      newBullet->posY = player.posY;
      newBullet->lastMoveTime = 0;
      ajouter_fin(&bullets, newBullet);
    }

  }


  bulletsTick(actualTime);
  invidersTick(actualTime);

}

void startMenuTick(DWORD actualTime) {

  while (_kbhit()) {          // une touche est pressée ?
    char c = _getch();        // lire la touche sans attendre Enter
    
    if (c == 75) { // Fleche gauche
      startMenuBtnIndex = 0;
    } else if (c == 77) { // Fleche droite
      startMenuBtnIndex = 1;
    } else if (c == 'q') {
      game.playing = 0;
      return;
    } else if (c == 13) { // enter

      if (startMenuBtnIndex == 0) {
        startGame();
      } else {
        game.playing = 0;
      }

    }

  }


}

void endMenuTick(DWORD actualTime) {

  while (_kbhit()) {          // une touche est pressée ?
    char c = _getch();        // lire la touche sans attendre Enter
    
    if (c == 13) { // enter
      
      game.playing = 1;
        
    }
  }
}

void tick() {

  DWORD actualTime = GetTickCount();

  switch (game.playing) {
    case 1:
      startMenuTick(actualTime);
      break;

    case 2:
      gameTick(actualTime);
      break;

    case 3:
      endMenuTick(actualTime);
      break;
    
    default:
      startMenuTick(actualTime);
      break;
  }

  if (actualTime - game.lastTickMs > 1000/MAX_FPS) {
    game.lastTickMs = GetTickCount();

    draw();
  }
  
}


void init() {
  startGame();
  
  game.playing = 2;

  startMenuBtnIndex = 0;

  hideCursor();
  drawAll();
}


int main() {
  srand(time(NULL));
  init();

  while (game.playing) {
    tick();
  }

  return 0;
}