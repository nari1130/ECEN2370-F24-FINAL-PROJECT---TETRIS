/*
 * ApplicationCode.h
 *
 *  Created on: Dec 30, 2023
 *      Author: Xavion
 */
#ifndef INC_APPLICATIONCODE_H_
#define INC_APPLICATIONCODE_H_

#include "MainMenu.h"
#include "GamePlay.h"
#include "Scheduler.h"
#include "Timer_Driver.h"
#include "stm32f4xx_hal.h"

#define	LENGTH_FN					6
#define DEFAULT_DELAY				25000



void ApplicationInit(void);
void applicationButtonInitInterrupt(void);
void screenLayers(void);
void LCD_Visual_Demo(void);
void MainMenu_Draw(void);
void App_Gameplay(void);
void App_LCD_ScreenLayers(void);
void appDelay(uint32_t x);
void appDelay(uint32_t x);


#define	LENGTH_FN					6
#define DEFAULT_DELAY				25000


#if (COMPILE_TOUCH_FUNCTIONS == 1) && (COMPILE_TOUCH_INTERRUPT_SUPPORT == 0)
void LCD_Touch_Polling_Demo(void);
#endif // (COMPILE_TOUCH_FUNCTIONS == 1) && (COMPILE_TOUCH_INTERRUPT_SUPPORT == 0)

#endif /* INC_APPLICATIONCODE_H_ */
