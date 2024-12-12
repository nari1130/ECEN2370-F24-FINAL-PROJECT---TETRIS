/*
 * MainMenu.h
 *
 *  Created on: Dec 3, 2024
 *      Author: poofy
 */

#ifndef SRC_MAINMENU_H_
#define SRC_MAINMENU_H_

#include "LCD_Driver.h"
#include "Button_Driver.h"

//  I
// 1111
// 0000
// 0000
// 0000

//  O
// 1100
// 1100
// 0000
// 0000

//  T
// 1110
// 0100
// 0000
// 0000

//  L
// 1000
// 1000
// 1110
// 0000

//  J
// 0010
// 0010
// 1110
// 0000

//  S
// 0110
// 1100
// 0000
// 0000

//  Z
// 1100
// 0110
// 0000
// 0000

// MAIN MENU
// 1. PLAY
// 2. QUIT

void DisplayTitle(void);

void DrawMainMenu(void);
void ButtonToStart(void);

#endif /* SRC_MAINMENU_H_ */
