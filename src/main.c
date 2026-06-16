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

#include "list.h"
#include "player.h"
#include "visual.h"
#include "invider.h"


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


typedef struct Game {
  int lastTickMs;
  int lastSpawnMs;
  //boolean moved;
  uint8_t playing; // 0 -> stop ; 1 -> start menu ; 2 -> play ; 3 -> menu end
} Game;


ListDoubleChaine inviders;
ListDoubleChaine bullets;

Player player;
Game game;

uint8_t startMenuBtnIndex = 0;

int bestScore = -1;


void clearRect(int x1, int y1, int width, int height) {
  for (int y = 0; y < height; y++) {
    moveTo(x1, y1+y);

    for (int x = 0; x < width; x++) putchar(' ');
  }
}

DoubleChaine* deleteInvider(DoubleChaine *inviderElement) {
  free(inviderElement->value);

  DoubleChaine *nextElement = inviderElement->next;

  supprimer(&inviders, inviderElement);

  return nextElement;
}

DoubleChaine* deleteBullet(DoubleChaine *bulletElement) {
  free(bulletElement->value);

  DoubleChaine *nextElement = bulletElement->next;

  supprimer(&bullets, bulletElement);

  return nextElement;
}

DoubleChaine* checkBulletCollision(DoubleChaine *bulletElement) {
  Bullet *bullet = bulletElement->value;
  DoubleChaine *currentElement = inviders.first;

  while (currentElement != NULL) {

    Invider *invider = currentElement->value;

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

DoubleChaine* checkInviderCollision(DoubleChaine *inviderElement) {
  Invider *invider = inviderElement->value;
  DoubleChaine *currentElement = bullets.first;

  while (currentElement != NULL) {

    Bullet *bullet = currentElement->value;

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
  DoubleChaine *currentElement = bullets.first;
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


/*void drawGame() {
  // clear game
  // light_gray_background();
  clearRect(OFFSET_X, OFFSET_Y, WIDTH, HEIGHT);



  // draw bullets
  DoubleChaine *currentBullet = bullets.first;

  while (currentBullet != NULL) {
    Bullet *bullet = currentBullet->value;

    moveToGame(bullet->posX, bullet->posY);
    blue_text();
    putchar('*');

    currentBullet = currentBullet->next;
  }


  // draw inviders
  DoubleChaine *currentInvider = inviders.first;

  while (currentInvider != NULL) {
    Invider *invider = currentInvider->value;

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
}*/

// drawStartMenu

/*void drawEndMenu() {
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

}*/

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

  switch (game.playing) {
    case 1:
      drawStartMenu(startMenuBtnIndex);
      break;

    case 2:
      drawGame(&player, &inviders, &bullets);
      break;

    case 3:
      drawEndMenu(player.score, getBestScore());
      break;
    
    default:
      drawStartMenu(startMenuBtnIndex);
      break;
  }

  reset();
  moveToGame(-OFFSET_X, -OFFSET_Y+1);
}

void startMenuEvent(SDL_Event* event) {
  if (event->type == SDL_EVENT_KEY_DOWN) {
    if (event->key.key == SDLK_LEFT) {
      startMenuBtnIndex = 0;
    } else if (event->key.key == SDLK_RIGHT) {
      startMenuBtnIndex = 1;
    } else if (event->key.key == SDLK_RETURN) {
      if (startMenuBtnIndex == 0) {
        startGame();
      } else {
        game.playing = 0;
      }
    }
  }
}

void gameEvent(SDL_Event* event) {
  if (event->type == SDL_EVENT_KEY_DOWN) {
    if (event->key.key == SDLK_LEFT) {
      playerMoveLeft(&player, 0, 1);
    } else if (event->key.key == SDLK_RIGHT) {
      playerMoveRight(&player, WIDTH-1, 1);
    } else if (event->key.key == SDLK_SPACE) {
      Bullet *newBullet = malloc(sizeof(Bullet));
      newBullet->posX = player.posX;
      newBullet->posY = player.posY;
      newBullet->lastMoveTime = 0;
      ajouter_fin(&bullets, newBullet);
    }
  }
}

void endMenuEvent(SDL_Event* event) {
  if (event->type == SDL_EVENT_KEY_DOWN) {
    if (event->key.key == SDLK_RETURN) {
      endGame();
    }
  }
}

void manageEvent(SDL_Event* event) {
  switch (game.playing) {
    case 1:
      startMenuEvent(event);
      break;

    case 2:
      gameEvent(event);
      break;

    case 3:
      endMenuEvent(event);
      break;
    
    default:
      startMenuEvent(event);
      break;
  }
}


void bulletsTick(DWORD actualTime) {
  DoubleChaine *currentElement = bullets.first;

  while (currentElement != NULL) {

    Bullet *bullet = currentElement->value;

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
    Invider *newInvider = malloc(sizeof(Invider));
    newInvider->posX = rand() % WIDTH;
    newInvider->posY = HEIGHT;
    newInvider->health = INVIDER_HEALTH;
    newInvider->lastMoveTime = 0;
    ajouter_fin(&inviders, newInvider);

    game.lastSpawnMs = actualTime;
  }

  DoubleChaine *currentElement = inviders.first;

  while (currentElement != NULL) {

    Invider *invider = currentElement->value;

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

  /*while (_kbhit()) {          // une touche est pressée ?
    char c = _getch();        // lire la touche sans attendre Enter
    
    if (c == 75) { // Fleche gauche
      playerMoveLeft(&player, 0, 1);
    } else if (c == 77) { // Fleche droite
      playerMoveRight(&player, WIDTH-1, 1);
    } else if (c == 'q') {
      game.playing = 0;
      return;
    } else if (c == ' ') {
      Bullet *newBullet = malloc(sizeof(Bullet));
      newBullet->posX = player.posX;
      newBullet->posY = player.posY;
      newBullet->lastMoveTime = 0;
      ajouter_fin(&bullets, newBullet);
    }

  }*/


  bulletsTick(actualTime);
  invidersTick(actualTime);

}

void startMenuTick(DWORD actualTime) {

  /*while (_kbhit()) {          // une touche est pressée ?
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

  }*/

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
  
  game.playing = 1;

  startMenuBtnIndex = 0;

}


int main() {
  srand(time(NULL));


  initGameVisual(WIDTH, HEIGHT, OFFSET_X, OFFSET_Y, PLAYER_HEALTH);
  initWindow();

  init();


  SDL_Event event;

  while (game.playing) {
    
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        game.playing = 0;
      }

      if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_Q) {
          game.playing = 0;
          continue;
        }
      }

      manageEvent(&event);
    }

    tick();

  }

  destroyWindow();

  return 0;
}