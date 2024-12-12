/*
 * InterruptControl.c
 *
 *  Created on: Oct 3, 2024
 *      Author: poofy
 */

#include "InterruptControl.h"

uint8_t Tetriminos[NUMBER_OF_TETRIMINOS][4][4] = {
		{{1, 1, 1, 1},
		 {0, 0, 0, 0},				//I
		 {0, 0, 0, 0},
		 {0, 0, 0, 0}},

		{{0, 0, 0, 0},
		 {0, 1, 1, 0},				//0
		 {0, 1, 1, 0},
		 {0, 0, 0, 0}},

		{{0, 1, 1, 1},
		 {0, 0, 1, 0},					//T
		 {0, 0, 0, 0},
		 {0, 0, 0, 0}},

		{{0, 0, 1, 0},
		 {0, 0, 1, 0},			//L
		 {0, 0, 1, 1},
		 {0, 0, 0, 0}},


		{{0, 0, 1, 0},
		 {0, 0, 1, 0},			//J
		 {0, 1, 1, 0},
		 {0, 0, 0, 0}},

		{{0, 1, 1, 0},
	     {1, 1, 0, 0},			//S
	     {0, 0, 0, 0},
	     {0, 0, 0, 0}},

		{{1, 1, 0, 0},
		 {0, 1, 1, 0},			//Z
		 {0, 0, 0, 0},
		 {0, 0, 0, 0}},
};


void IRQ_EI(uint8_t IRQnum){					//enable interrupt
	HAL_NVIC_EnableIRQ((IRQn_Type)IRQnum);
}
void IRQ_DI(uint8_t IRQnum){					//disable interrupt
	HAL_NVIC_DisableIRQ((IRQn_Type)IRQnum);

}
void IRQ_SPI(uint8_t IRQnum){						//SET PENDING INTERRUPT
	HAL_NVIC_SetPendingIRQ((IRQn_Type)IRQnum);
}
void IRQ_CPI(uint8_t IRQnum){							//CLEAR PENDING INTERRUPT
	HAL_NVIC_ClearPendingIRQ((IRQn_Type)IRQnum);
}
void EXTI_CPIB(uint8_t PinNum){						//clear pending interrupt pin
	 __HAL_GPIO_EXTI_CLEAR_IT(PinNum);
}

