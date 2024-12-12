/*
 * Gameplay.c
 *
 *  Created on: Dec 1, 2024
 *      Author: poofy
 */
#include "GamePlay.h"
//ROTATE TETRIMINO, TRANSPOSE MATRIX, REVERSE ROWS

uint8_t currentTetrimino[4][4];

int N;
int currentX = 120;
int currentY = 0;           //TETRIMINOS WILL GENERATE HERE
int currentIndex = 0;

int singles = 0;
int doubles = 0;
int triples = 0;
int tetrises = 0;
int score = 0;

bool lastTetriminoIsPlaced = true;
bool gameOver = false;

int board[GRID_HEIGHT][GRID_WIDTH] = {0};


void generateTetrimino(void){
	if (lastTetriminoIsPlaced == true){
		if(checkforCollision(currentX, currentY-20)){
			gameOver = true;
			addSchedulerEvent(END_GAME_EVENT);
			return;
		}
	RNG_Init();
	int rando = RNG_Generate();
	DrawTetriminos(rando, currentX, currentY);
	currentIndex = rando;
		//COPY CONTENTS OF GENERATED TETRIMINO TO currentTetrimino
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			currentTetrimino[i][j] = Tetriminos[currentIndex][i][j];
		}
	}
	currentX = 120;
	currentY = 0;
	lastTetriminoIsPlaced = false;

	HAL_Delay(600);
	}
	else {
		return;
	}
}

bool checkforCollision(int x, int y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (currentTetrimino[i][j] == 1) {
                int newX = x + j*20;
                int newY = y + i*20;
                int newXGrid = newX / 20;
                int newYGrid = newY / 20;

                if (newXGrid < 0 || newXGrid >= GRID_WIDTH || newYGrid >= GRID_HEIGHT || board[newYGrid][newXGrid] != 0) {
                    return true;
                }
            }
        }
    }
    return false; //no collision
}

void rotateTetriminoCW(void){
	uint8_t tempTetrimino[4][4];
	for (int i = 0; i < 4; i++) {		//TRANSPOSE MATRIX
		for (int j = 0; j < 4; j++) {
	            tempTetrimino[j][i] = currentTetrimino[i][j];
	    }
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0, k = 3; j < k; j++, k--) {
		    int temp = tempTetrimino[i][j];
		    tempTetrimino[i][j] = tempTetrimino[i][k];
		    tempTetrimino[i][k] = temp;
		}
	}
	for (int i = 0; i < 4; i++) {							//copy transposed, reversed rows NEW MATRIX
		 for (int j = 0; j < 4; j++) {
			 currentTetrimino[i][j] = tempTetrimino[i][j];
	     }
	}
}

void rotateTetrimino(void){
	uint8_t tempTetrimino[4][4];
	clearTetrimino(currentX, currentY);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tempTetrimino[i][j] = currentTetrimino[i][j];
		}
	}
	rotateTetriminoCW();
	if (checkforCollision(currentX, currentY)){
		for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					currentTetrimino[i][j] = tempTetrimino[i][j];
				}
			}
	}
	else {
		DrawTetrimino(currentX, currentY, currentTetrimino, TetriminoColors(currentIndex));

	}


}

void placeTetrimino(void) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (currentTetrimino[i][j] == 1) {		//
                int newX = currentX + j*20;
                int newY = currentY + i*20;
                int newXGrid = newX / 20;
                int newYGrid = newY / 20;

                if (newYGrid >= 0 && newYGrid < GRID_HEIGHT && newXGrid >= 0 && newXGrid < GRID_WIDTH) {
                    board[newYGrid][newXGrid] = 1; //mark position on 12*16 grid as occupied

                }
            }
        }
    }
    currentX = 120;
    currentY = 0;
    clearRows();
}

void updateTetriminoPosition(int touchX, int touchY) {
	int moveThreshold = 20;

	int newX = currentX;
	int newY = currentY;

	if (touchX < currentX) {
		newX = currentX - moveThreshold;  //left
	} else if (touchX > currentX) {
		newX = currentX + moveThreshold;  //right
	}

	if (!checkforCollision(newX, newY)) {
		clearTetrimino(currentX, currentY);
		currentX = newX;
		currentY = newY;
		DrawTetrimino(currentX, currentY, currentTetrimino, TetriminoColors(currentIndex));
	}
	else {
	       return;
	    }
}



void clearTetrimino(int x, int y){
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (currentTetrimino[i][j] == 1) { //check if cell is part of tetrimino
	        	int blockX = x + j * 20; 		//grid coordinates to pixel coordinates
	        	int blockY = y + i * 20;
	        	LCD_Draw_Rectangle_Fill(blockX, blockY, 20, 20, LCD_COLOR_BLACK);
	     	}
		}
	}
	LCD_DrawGrid();
}

void generateGameGrid(void){
	LCD_Clear(ENDSCREEN_LAYER, LCD_COLOR_BLACK);
	LCD_DrawLayer(GAMEPLAY_LAYER);
}

void moveTetriminoDown(){
	if(!lastTetriminoIsPlaced){
	if (checkforCollision(currentX, currentY + 20)){//IF THERE IS A COLLISION
			placeTetrimino();
			lastTetriminoIsPlaced = true;
			removeSchedulerEvent(MOVE_TETRIMINO_DOWN_EVENT);
			addSchedulerEvent(GENERATE_TETRIMINO_EVENT);
			return;
	}
	else{
		clearTetrimino(currentX, currentY);
		currentY+=20;
	    DrawTetrimino(currentX, currentY, currentTetrimino, TetriminoColors(currentIndex));
		HAL_Delay(600);
	}
	}
}

void gameOverFunction(void){
	if (gameOver){
		stopTimer();
		LCD_DrawLayer(ENDSCREEN_LAYER);
		removeSchedulerEvent(END_GAME_EVENT);
	}
	return;
}

void clearRows(void) {
    int clearedRows = 0;
    for (int y = 0; y < GRID_HEIGHT; y++) {
        bool fullRow = true;
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (board[y][x] == 0) {
                fullRow = false;
                break;
            }
        }
        if (fullRow) {
            clearedRows++;
            for (int i = y; i > 0; i--) {
                for (int j = 0; j < GRID_WIDTH; j++) {
                    board[i][j] = board[i - 1][j];
                }
            }
            for (int j = 0; j < GRID_WIDTH; j++) {
                board[0][j] = 0; 						//row becomes empty
            }
        }
    }
    switch (clearedRows) {             //counters
        case 1:
        	singles++;
        	score+= 40;
        	break;
        case 2:
        	doubles++;
        	score+= 100;
        	break;
        case 3:
        	triples++;
        	score+= 300;
        	break;
        case 4:
        	tetrises++;
        	score+= 1200;
        	break;
        default:
        	break;
    }
}


void updateScore(int points){
	score += points;
}

void drawEndScreen(void){
	LCD_Clear(0, LCD_COLOR_PINK);
	LCD_DisplayChar(50, 0, 'G');
	LCD_DisplayChar(65, 0, 'A');
	LCD_DisplayChar(80, 0, 'M');
	LCD_DisplayChar(95, 0, 'E');

	LCD_DisplayChar(125, 0, 'O');
	LCD_DisplayChar(140, 0, 'V');
	LCD_DisplayChar(155, 0, 'E');
	LCD_DisplayChar(170, 0, 'R');
	LCD_DisplayChar(180, 0, '!');

	LCD_DisplayChar(20, 20, 'S');
	LCD_DisplayChar(35, 20, 'i');
	LCD_DisplayChar(50, 20, 'n');
	LCD_DisplayChar(65, 20, 'g');
	LCD_DisplayChar(80, 20, 'l');
	LCD_DisplayChar(95, 20, 'e');
	LCD_DisplayChar(110, 20, 's');
	LCD_DisplayChar(120, 20, ':');

	LCD_DisplayInt(135, 20, singles);

	LCD_DisplayChar(20, 40, 'D');
	LCD_DisplayChar(35, 40, 'o');
	LCD_DisplayChar(50, 40, 'u');
	LCD_DisplayChar(65, 40, 'b');
	LCD_DisplayChar(80, 40, 'l');
	LCD_DisplayChar(95, 40, 'e');
	LCD_DisplayChar(110, 40, 's');
	LCD_DisplayChar(125, 40, ':');

	LCD_DisplayInt(135, 20, doubles);

	LCD_DisplayChar(20, 60, 'T');
	LCD_DisplayChar(35, 60, 'e');
	LCD_DisplayChar(50, 60, 't');
	LCD_DisplayChar(65, 60, 'r');
	LCD_DisplayChar(80, 60, 'i');
	LCD_DisplayChar(95, 60, 's');
	LCD_DisplayChar(110, 60, 'e');
	LCD_DisplayChar(125, 60, 's');
	LCD_DisplayChar(135, 60, ':');

	LCD_DisplayInt(135, 20, tetrises);

	LCD_DisplayChar(20, 100, 'T');
	LCD_DisplayChar(35, 100, 'O');
	LCD_DisplayChar(50, 100, 'T');
	LCD_DisplayChar(65, 100, 'A');
	LCD_DisplayChar(80, 100, 'L');

	LCD_DisplayChar(110, 100, 'S');
	LCD_DisplayChar(125, 100, 'C');
	LCD_DisplayChar(140, 100, 'O');
	LCD_DisplayChar(155, 100, 'R');
	LCD_DisplayChar(170, 100, 'E');
	LCD_DisplayChar(180, 100, ':');

	LCD_DisplayInt(195, 100, score);

	LCD_DisplayChar(50, 140, 'T');
	LCD_DisplayChar(60, 140, 'I');
	LCD_DisplayChar(70, 140, 'M');
	LCD_DisplayChar(85, 140, 'E');
	LCD_DisplayChar(100, 140, ':');

	uint32_t x = GetElapsedTime() / 8;
	LCD_DisplayInt(120, 140, x);


	LCD_DisplayChar(170, 140, 's');
	LCD_DisplayChar(180, 140, 'e');
	LCD_DisplayChar(190, 140, 'c');
	LCD_DisplayChar(200, 140, 's');
}



