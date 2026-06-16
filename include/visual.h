#ifndef VISUAL_H
#define VISUAL_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include "invider.h"
#include "bullet.h"
#include "player.h"
#include "list.h"



void initGameVisual(uint16_t _gameWidth, uint16_t _gameHeight, uint16_t _xOffset, uint16_t _yOffset, uint16_t _playerMaxHealth);
void initWindow();

void destroyWindow();

void drawTextAt(int x, int y, const char* text, TTF_Font* font, SDL_Color* color, uint8_t xAnchor, uint8_t yAnchor, SDL_Color* highlightColor);
void drawTextureAt(int x, int y, int width, SDL_Texture* texture, uint8_t xAnchor, uint8_t yAnchor);
void drawRectAt(int x, int y, int width, int height, SDL_Color* color);

void getGamePosition(int gameX, int gameY, int* posX, int* posY);

void drawGame(Player* player, ListDoubleChaine* inviders, ListDoubleChaine* bullets);
void drawStartMenu(uint8_t btnIndex);
void drawEndMenu(uint16_t score, uint16_t bestScore);

#endif