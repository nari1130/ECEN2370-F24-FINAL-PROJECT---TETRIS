/*
 * GamePlay.h
 *
 *  Created on: Dec 1, 2024
 *      Author: poofy
 */

#ifndef INC_GAMEPLAY_H_
#define INC_GAMEPLAY_H_

#include "LCD_Driver.h"
#include "Button_Driver.h"
#include "Scheduler.h"
#include "Timer_Driver.h"


#endif /* INC_GAMEPLAY_H_ */
#define GRID_WIDTH 		12
#define GRID_HEIGHT 	16

#define SCREEN_HEIGHT	320
#define SCREEN_WIDTH 	240

#define FRAMES_PER_LAYER = 180

void playGame(void);
void generateTetrimino(void);
bool checkforCollision(int x, int y);
void placeTetrimino(void);
void playTetrimino(void);
void rotateTetriminoCW(void);
void rotateTetrimino(void);
void generateGameGrid(void);
void moveTetriminoDown(void);
void gameOverFunction(void);
void updateTetriminoPosition(int touchX, int touchY);

void gameOverFunction(void);
void clearRows (void);
void drawEndScreen(void);



void clearTetrimino(int x, int y);



