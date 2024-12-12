/*
 * Timer_Driver.c
 *
 *  Created on: Oct 10, 2024
 *      Author: poofy
 */


#include "Timer_Driver.h"

static TIM_HandleTypeDef htim2;

int elapsedTime = 0;

void Timer_Init(void){					//initialize the specified timer,
		__HAL_RCC_TIM2_CLK_ENABLE();
		htim2.Instance = TIM2;
		htim2.Init.Prescaler = 0;
		htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
		htim2.Init.Period = 999;
		htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;


	    HAL_TIM_Base_Init(&htim2);
}

void StartTimer(void){
	elapsedTime = 0;
	HAL_TIM_Base_Start(&htim2);
}

void stopTimer(void){
	HAL_TIM_Base_Stop(&htim2);
}

uint32_t TimerValue(TIM_TypeDef *conTimReg){
		return conTimReg->CNT;
}
uint32_t GetElapsedTime(void){
	 return (double)(htim2.Instance->CNT);
}



