/*
 * Button_Driver.h
 *
 *  Created on: Sep 24, 2024
 *      Author: poofy
 */

#ifndef BUTTON_DRIVER_H_
#define BUTTON_DRIVER_H_

#include "InterruptControl.h"
#include "stm32f4xx_hal.h"


#define BUTTON_PORT 	GPIOA
#define BUTTON_PIN 		GPIO_PIN_0

#define BUTTON_DOWN 	1
#define BUTTON_UP		0

bool ButtonPressed(void);
void Button_Init_Interrupt(void);
void CheckUserButton(void);
#endif /* BUTTON_DRIVER_H_ */
