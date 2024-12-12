/*
 * InterruptControl.h
 *
 *  Created on: Oct 3, 2024
 *      Author: poofy
 */

#ifndef INTERRUPTCONTROL_H_
#define INTERRUPTCONTROL_H_

#include "stm32f4xx_hal.h"
#include "LCD_Driver.h"


#define EXTI0_IRQ_NUMBER		6			//button interrupt
#define TIM2_IRQ_NUMBER			28
#define EXTI15_IRQ_NUMBER		88			//touch screen interrupt

#define NUMBER_OF_TETRIMINOS	7

extern uint8_t Tetriminos[NUMBER_OF_TETRIMINOS][4][4];

typedef enum {
	SCREEN_MAIN_MENU,
    SCREEN_GAMEPLAY,
    SCREEN_GAME_OVER
} ScreenState;

typedef enum {
	GAME_INIT,
    GAME_GENERATE_TETRIMINO,
	GAME_PLAY_TETRIMINO,
	GAME_ISOVER
} GameState;

//all take in a uint8_t argument (IRQ number), return nothing
void IRQ_EI(uint8_t IRQnum);					//ENABLE INTERRUPT
void IRQ_DI(uint8_t IRQnum);					//DISBALE INTERRUPT
void IRQ_CPI(uint8_t IRQnum);					//CLEAR PENDING INTERRUPT
void IRQ_SPI(uint8_t IRQnum);					//SET PENDING INTERRUPT

void EXTI_CPIB(uint8_t PinNum);					//clear the interrupt pending BIT in the EXTI interrupt register, takes in pin num



#endif /* INTERRUPTCONTROL_H_ */
