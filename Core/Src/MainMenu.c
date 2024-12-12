/*
 * MainMenu.c
 *
 *  Created on: Dec 3, 2024
 *      Author: poofy
 */

#include "MainMenu.h"


void DisplayTitle(void){
	LCD_DisplayChar(75, 0, 'T');
	LCD_DisplayChar(90, 0, 'E');
	LCD_DisplayChar(105, 0, 'T');
	LCD_DisplayChar(120, 0, 'R');
	LCD_DisplayChar(130, 0, 'I');
	LCD_DisplayChar(140, 0, 'S');
}

void ButtonToStart(void){
	LCD_DisplayChar(15, 250, 'U');
	LCD_DisplayChar(30, 250, 'S');
	LCD_DisplayChar(45, 250, 'E');
	LCD_DisplayChar(60, 250, 'R');

	LCD_DisplayChar(90, 250, 'B');
	LCD_DisplayChar(105, 250, 'U');
	LCD_DisplayChar(120, 250, 'T');
	LCD_DisplayChar(135, 250, 'T');
	LCD_DisplayChar(150, 250, 'O');
	LCD_DisplayChar(165, 250, 'N');

	LCD_DisplayChar(195, 250, 'T');
	LCD_DisplayChar(210, 250, 'O');

	LCD_DisplayChar(75, 280, 'S');
	LCD_DisplayChar(90, 280, 'T');
	LCD_DisplayChar(105, 280, 'A');
	LCD_DisplayChar(120, 280, 'R');
	LCD_DisplayChar(135, 280, 'T');
	LCD_DisplayChar(150, 280, '!');
}

void DrawMainMenu(void){
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_SetFont(&Font16x24);

	DisplayTitle();

	MainTetriminoes();
	ButtonToStart();
}
