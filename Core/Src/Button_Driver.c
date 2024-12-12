/*
 * Button_Driver.c
 *
 *  Created on: Sep 24, 2024
 *      Author: poofy
 */
#include "Button_Driver.h"


bool ButtonPressed(){
	return HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == GPIO_PIN_SET;
}

void Button_Init_Interrupt(void){
	GPIO_InitTypeDef ButtonConfig = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	ButtonConfig.Pin = BUTTON_PIN;
	ButtonConfig.Mode = GPIO_MODE_IT_RISING;
	ButtonConfig.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(BUTTON_PORT, &ButtonConfig);

	HAL_NVIC_SetPriority(EXTI0_IRQ_NUMBER, 2, 0);          		////PARAMETERS: IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority
	HAL_NVIC_EnableIRQ(EXTI0_IRQ_NUMBER);
}




