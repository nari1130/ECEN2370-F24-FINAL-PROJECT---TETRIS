/*
 * RNG.h
 *
 *  Created on: Dec 1, 2024
 *      Author: poofy
 */

#ifndef INC_RNG_H_
#define INC_RNG_H_

#include "stm32f4xx_hal.h"
#include "Scheduler.h"


void RNG_Init(void);
void RNG_Error_Handler(void);
uint32_t RNG_Generate(void);
char RNGAscii(void);


#endif /* INC_RNG_H_ */
