/*
 * RNG.c
 *
 *  Created on: Dec 1, 2024
 *      Author: poofy
 */

#include "RNG.h"

static RNG_HandleTypeDef hrng;

void RNG_Init(void){
	__HAL_RCC_RNG_CLK_ENABLE();
	hrng.Instance = RNG;

	if (HAL_RNG_Init(&hrng) != HAL_OK){
		RNG_Error_Handler();
	}
}
void RNG_Error_Handler(void){
	/* USER CODE BEGIN Error_Handler_Debug */
	  /* User can add his own implementation to report the HAL error return state */
	  __disable_irq();
	  while (1)
	  {
	  }
}
uint32_t RNG_Generate(void) {
    uint32_t randomValue;
    hrng.State == HAL_RNG_STATE_READY;
    if (HAL_RNG_GenerateRandomNumber(&hrng, &randomValue) == HAL_OK) {
        return (int)(randomValue % 7);
    }
    else{
    	return 0;
    }
}

char RNGAscii(void){                           //TESTING
	uint32_t rando = RNG_Generate();
	if (rando != 0){
	        return (char)(32 + (rando % 95));
	}
	else{
		return 'X';
	}
}
