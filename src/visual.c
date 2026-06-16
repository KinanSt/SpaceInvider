#include "visual.h"


SDL_Window *window;
SDL_Renderer *renderer;

SDL_Surface *icon;

TTF_Font *font;
TTF_Font *gameFont;

SDL_Texture *playerTexture;
SDL_Texture *inviderTexture;

SDL_Color white;
SDL_Color green;
SDL_Color red;
SDL_Color blue;
SDL_Color black;
SDL_Color lightGray;

uint16_t actualFontSize = 0;

int windowWidth, windowHeight;
uint16_t caseSize, usedWidth, usedHeight, gameWidth, gameHeight, xOffset, yOffset, playerMaxHealth;


void initGameVisual(uint16_t _gameWidth, uint16_t _gameHeight, uint16_t _xOffset, uint16_t _yOffset, uint16_t _playerMaxHealth) {
  gameWidth = _gameWidth;
  gameHeight = _gameHeight;
  xOffset = _xOffset;
  yOffset = _yOffset;
  playerMaxHealth = _playerMaxHealth;
}

void initWindow() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL_Init: %s\n", SDL_GetError());
    return;
  }
  
  if (TTF_Init() < 0) {
    printf("TTF_Init: %s\n", SDL_GetError());
    return;
  }

  window = SDL_CreateWindow("Space Invider", 1000, 1000, SDL_WINDOW_RESIZABLE);

  if (window == NULL) {
    printf("CreateWindow: %s\n", SDL_GetError());
    exit(1);
  }


  icon = IMG_Load("imgs/player.png");

  if (icon) {
    SDL_SetWindowIcon(window, icon);
    SDL_DestroySurface(icon);
  }

  renderer = SDL_CreateRenderer(window, NULL);

  if (renderer == NULL) {
    printf("CreateRenderer: %s\n", SDL_GetError());
    return;
  }

  font = TTF_OpenFont("fonts/arial.ttf", 24);
  if (!font) {
    printf("Font error: %s\n", SDL_GetError());
  }

  playerTexture = IMG_LoadTexture(renderer, "imgs/player.png");

  if (!playerTexture) {
    printf("Erreur image : %s\n", SDL_GetError());
  }

  inviderTexture = IMG_LoadTexture(renderer, "imgs/invader.png");

  if (!inviderTexture) {
    printf("Erreur image : %s\n", SDL_GetError());
  }
  
  white = (SDL_Color){255, 255, 255, 255};
  green = (SDL_Color){50, 150, 50, 255};
  red = (SDL_Color){255, 0, 0, 255};
  blue = (SDL_Color){100, 100, 255, 255};
  black = (SDL_Color){0, 0, 0, 255};
  lightGray = (SDL_Color){100, 100, 100, 255};
}

void destroyWindow() {
  SDL_DestroyWindow(window);
  SDL_Quit();
}


void drawTextAt(int x, int y, const char* text, TTF_Font* font, SDL_Color* color, uint8_t xAnchor, uint8_t yAnchor, SDL_Color* highlightColor) {

  if (!font) {
    printf("Font error\n");
    return;
  }

  SDL_Surface *surface = TTF_RenderText_Blended(font, text, strlen(text), *color);

  if (!surface) {
    printf("Surface error: %s\n", SDL_GetError());
    return;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  if (!texture) {
    printf("Texture error: %s\n", SDL_GetError());
    SDL_DestroySurface(surface);
    return;
  }
  

  int16_t xOffset = 0;
  int16_t yOffset = 0;

  if (xAnchor == 1) {
    xOffset = surface->w/2;
  } else if (xAnchor == 2) {
    xOffset = surface->w;
  }

  if (yAnchor == 1) {
    yOffset = surface->h/2;
  } else if (yAnchor == 2) {
    yOffset = surface->h;
  }

  SDL_FRect rect = {x-xOffset, y-yOffset, surface->w, surface->h};

  if (highlightColor) {
    uint8_t highlightPadding = 5;
    drawRectAt(rect.x - highlightPadding, rect.y-highlightPadding, rect.w+highlightPadding*2, rect.h+highlightPadding*2, highlightColor);
  }
  
  SDL_RenderTexture(renderer, texture, NULL, &rect);

  SDL_DestroyTexture(texture);
  SDL_DestroySurface(surface);

}

void drawTextureAt(int x, int y, int width, SDL_Texture* texture, uint8_t xAnchor, uint8_t yAnchor) {

  if (!texture) {
    printf("Texture error\n");
    return;
  }

  int newWidth = width;
  int newHeight = width*texture->h/texture->w;
  

  int16_t xOffset = 0;
  int16_t yOffset = 0;

  if (xAnchor == 1) {
    xOffset = newWidth/2;
  } else if (xAnchor == 2) {
    xOffset = newWidth;
  }

  if (yAnchor == 1) {
    yOffset = newHeight/2;
  } else if (yAnchor == 2) {
    yOffset = newHeight;
  }

  SDL_FRect dst = {x-xOffset, y-yOffset, newWidth, newHeight};

  SDL_RenderTexture(renderer, texture, NULL, &dst);

}

void drawRectAt(int x, int y, int width, int height, SDL_Color* color) {

  const SDL_FRect rect = {
    .x = x,
    .y = y,
    .w = width,
    .h = height
  };


  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
  SDL_RenderFillRect(renderer, &rect);
}


void getGamePosition(int gameX, int gameY, int* posX, int* posY) {
  *posX = (windowWidth-gameWidth*caseSize)/2+(gameX+0.5)*caseSize;

  *posY = (windowHeight-gameHeight*caseSize)/2+(gameHeight-(gameY+0.5))*caseSize;

  /*printf("DEBUG getGamePosition:\tgX:%d\tgY:%d\tx:%d\ty:%d\n",
    gameX, gameY,
    *posX,
    *posY);*/
}

TTF_Font* getGameFont() {
  if (actualFontSize != caseSize || gameFont==NULL) {
    TTF_CloseFont(gameFont);
    gameFont = TTF_OpenFont("fonts/arial.ttf", caseSize);
    actualFontSize = caseSize;

    if (!gameFont) {
      printf("Font error: %s\n", SDL_GetError());
    }
  }

  return gameFont;
}

void updateDrawVars() {
  if (((float)windowWidth)/windowHeight < (gameWidth+2.f*xOffset)/(gameHeight+2.f*yOffset)) {
    caseSize = windowWidth/(gameWidth+2*xOffset);
    usedWidth = windowWidth;
    usedHeight = caseSize*(gameHeight+2*yOffset);
  } else {
    caseSize = windowHeight/(gameHeight+2*yOffset);
    usedWidth = caseSize*(gameWidth+2*xOffset);
    usedHeight = windowHeight;
  }

  /*printf("DEBUG drawGame: window=%dx%d game=%dx%d offset=%d,%d caseSize=%d used=%dx%d\n",
    windowWidth, windowHeight,
    gameWidth, gameHeight,
    xOffset, yOffset,
    caseSize,
    usedWidth, usedHeight);*/
  /*fflush(stdout);
  system("pause");*/
}


void drawGame(Player* player, ListDoubleChaine* inviders, ListDoubleChaine* bullets) {
  SDL_GetWindowSize(window, &windowWidth, &windowHeight);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  drawRectAt((windowWidth-gameWidth*caseSize)/2, (windowHeight-gameHeight*caseSize)/2, gameWidth*caseSize, gameHeight*caseSize, &lightGray);
  
  updateDrawVars();

  int posX, posY;

  // draw bullets
  DoubleChaine *currentBullet = bullets->first;

  char* bulletSymbol = "*";

  while (currentBullet != NULL) {
    Bullet *bullet = currentBullet->value;


    getGamePosition(bullet->posX, bullet->posY, &posX, &posY);

    drawTextAt(posX, posY, bulletSymbol, getGameFont(), &red, 1, 1, NULL);

    currentBullet = currentBullet->next;
  }


  // draw inviders
  DoubleChaine *currentInvider = inviders->first;

  char* inviderSymbol = "$";

  while (currentInvider != NULL) {
    Invider *invider = currentInvider->value;

    getGamePosition(invider->posX, invider->posY, &posX, &posY);

    drawTextureAt(posX, posY, caseSize, inviderTexture, 1, 1);

    //drawTextAt(posX, posY, inviderSymbol, getGameFont(), &red, 1, 1, NULL);

    currentInvider = currentInvider->next;
  }

  // draw player
  
  getGamePosition(player->posX, player->posY, &posX, &posY);

  //char* playerSymbol = "^";
  //drawTextAt(posX, posY, playerSymbol, getGameFont(), &white, 1, 1, NULL);

  drawTextureAt(posX, posY, caseSize, playerTexture, 1, 1);


  getGamePosition(0, -2, &posX, &posY);
  char buffer[32];
  sprintf(buffer, "Score : %d", player->score);
  drawTextAt(posX-caseSize/2, posY, buffer, getGameFont(), &white, 0, 2, NULL);


  getGamePosition(0, -3, &posX, &posY);
  sprintf(buffer, "Vie : %d/%d", player->health, playerMaxHealth);
  drawTextAt(posX-caseSize/2, posY, buffer, getGameFont(), &white, 0, 2, NULL);

  /*printf("Score : %d", player.score);

  moveToGame(0, -3);
  printf("Vie : %d/%d", player.health, PLAYER_HEALTH);*/

  SDL_RenderPresent(renderer);
}

void drawStartMenu(uint8_t btnIndex) {
  SDL_GetWindowSize(window, &windowWidth, &windowHeight);

  updateDrawVars();

  int posX, posY;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  

  // drawTextAt(windowWidth/2, 50, "SpaceInvider by Kinan STROZYK", font, &white, 1, 0, NULL);
  getGamePosition(gameWidth/2, gameHeight*3/4, &posX, &posY);
  drawTextAt(posX, posY, "SpaceInvider by Kinan STROZYK", getGameFont(), &white, 1, 1, NULL);

  
  //drawTextAt(windowWidth/2 - 50, 200, "Play", font, &white, 2, 0, btnIndex==0?&green:NULL);
  getGamePosition(gameWidth/2 - 2, gameHeight/2, &posX, &posY);
  drawTextAt(posX, posY, "Play", getGameFont(), &white, 2, 1, btnIndex==0?&green:NULL);

  //drawTextAt(windowWidth/2 + 50, 200, "Quit", font, &white, 0, 0, btnIndex==1?&red:NULL);
  getGamePosition(gameWidth/2 +2, gameHeight/2, &posX, &posY);
  drawTextAt(posX, posY, "Quit", getGameFont(), &white, 0, 1, btnIndex==1?&red:NULL);

  SDL_RenderPresent(renderer);
  
}

void drawEndMenu(uint16_t score, uint16_t bestScore) {
  SDL_GetWindowSize(window, &windowWidth, &windowHeight);

  updateDrawVars();

  int posX, posY;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);


  getGamePosition(gameWidth / 2, gameHeight/2+4, &posX, &posY);
  drawTextAt(posX, posY, "You lose...", gameFont, &white, 1, 1, NULL);

  //moveToGame(WIDTH / 2 - (16 + (player.score > 0? (int) floor(log10(player.score)) + 1 : 0))/2, HEIGHT/2 + 3);
  //printf("Your score is : %d", player.score);
  char buffer[32];

  sprintf(buffer, "Your score is : %d", score);
  getGamePosition(gameWidth / 2, gameHeight/2 + 3, &posX, &posY);
  drawTextAt(posX, posY, buffer, gameFont, &white, 1, 1, NULL);

  //int _bestScore = getBestScore();
  //moveToGame(WIDTH / 2 - (16 + (_bestScore > 0? (int) floor(log10(_bestScore)) + 1 : 0))/2, HEIGHT/2 + 2);
  //printf("Best score is : %d", _bestScore);

  
  sprintf(buffer, "Best score is : %d", bestScore);
  getGamePosition(gameWidth / 2, gameHeight/2 + 2, &posX, &posY);
  drawTextAt(posX, posY, buffer, gameFont, &white, 1, 1, NULL);


  getGamePosition(gameWidth/2, gameHeight/2 - 2, &posX, &posY);
  drawTextAt(posX, posY, "Back", gameFont, &black, 1, 1, &white);
  
  

  SDL_RenderPresent(renderer);
  
}