/*
 * ApplicationCode.c
 *
 *  Created on: Dec 30, 2023 (updated 11/12/2024) Thanks Donavon! 
 *      Author: Xavion
 */

#include "ApplicationCode.h"

/* Static variables */

#define UIF_SET						1 << 0


extern void initialise_monitor_handles(void); 

static ScreenState currentScreen = SCREEN_MAIN_MENU;
static LTDC_HandleTypeDef hltdc;


void appDelay(uint32_t x){
	char name[LENGTH_FN] = {'N', 'a', 't', 'a', 'l', 'y'};
	[[maybe_unused]] char destinationArr[LENGTH_FN];

	for(volatile uint32_t j = 0;j <= x ; j++){											//count to x (input)
		for(volatile uint32_t i = 0; i < LENGTH_FN; i++){
				destinationArr[i] = name[i];
		}
	}
}

#if COMPILE_TOUCH_FUNCTIONS == 1
static STMPE811_TouchData StaticTouchData;
#if TOUCH_INTERRUPT_ENABLED == 1
static EXTI_HandleTypeDef LCDTouchIRQ;
void LCDTouchScreenInterruptGPIOInit(void);
#endif // TOUCH_INTERRUPT_ENABLED
#endif // COMPILE_TOUCH_FUNCTIONS

void ApplicationInit(void)
{
	initialise_monitor_handles(); // Allows printf functionality
	addSchedulerEvent(APP_DELAY_FLAG_EVENT);

    LTCD_Init();
	RNG_Init();
	Timer_Init();


    applicationButtonInitInterrupt();
    App_LCD_ScreenLayers();
    DrawMainMenu();


    #if COMPILE_TOUCH_FUNCTIONS == 1
	InitializeLCDTouch();

	// This is the orientation for the board to be direclty up where the buttons are vertically above the screen
	// Top left would be low x value, high y value. Bottom right would be low x value, low y value.
	StaticTouchData.orientation = STMPE811_Orientation_Portrait_2;

	#if TOUCH_INTERRUPT_ENABLED == 1
	LCDTouchScreenInterruptGPIOInit();
	#endif // TOUCH_INTERRUPT_ENABLED

	#endif // COMPILE_TOUCH_FUNCTIONS
}
void applicationButtonInitInterrupt(){
	Button_Init_Interrupt();
}

void LCD_Visual_Demo(void)
{
	visualDemo();
}

void App_LCD_ScreenLayers(void){
	LCD_ScreenLayers();
}


#if COMPILE_TOUCH_FUNCTIONS == 1
void LCD_Touch_Polling_Demo(void)
{
	LCD_Clear(0,LCD_COLOR_GREEN);
	while (1) {
		/* If touch pressed */
		if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
			/* Touch valid */
			printf("\nX: %03d\nY: %03d\n", StaticTouchData.x, StaticTouchData.y);
			LCD_Clear(0, LCD_COLOR_RED);
		} else {
			/* Touch not pressed */
			printf("Not Pressed\n\n");
			LCD_Clear(0, LCD_COLOR_GREEN);
		}
	}
}


// TouchScreen Interrupt
#if TOUCH_INTERRUPT_ENABLED == 1

void LCDTouchScreenInterruptGPIOInit(void)
{
	GPIO_InitTypeDef LCDConfig = {0};
    LCDConfig.Pin = GPIO_PIN_15;
    LCDConfig.Mode = GPIO_MODE_IT_RISING_FALLING;
    LCDConfig.Pull = GPIO_NOPULL;
    LCDConfig.Speed = GPIO_SPEED_FREQ_HIGH;
    
    // Clock enable
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // GPIO Init      
    HAL_GPIO_Init(GPIOA, &LCDConfig);

    // Interrupt Configuration
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	LCDTouchIRQ.Line = EXTI_LINE_15;

}

#define TOUCH_DETECTED_IRQ_STATUS_BIT   (1 << 0)  // Touchscreen detected bitmask

static uint8_t statusFlag;

void EXTI15_10_IRQHandler()
{
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn); // May consider making this a universial interrupt guard
	bool isTouchDetected = false;

	static uint32_t count;
	count = 0;
	while(count == 0){
		count = STMPE811_Read(STMPE811_FIFO_SIZE);
	}

	// Disable touch interrupt bit on the STMPE811
	uint8_t currentIRQEnables = ReadRegisterFromTouchModule(STMPE811_INT_EN);
	WriteDataToTouchModule(STMPE811_INT_EN, 0x00);

	// Clear the interrupt bit in the STMPE811
	statusFlag = ReadRegisterFromTouchModule(STMPE811_INT_STA);
	uint8_t clearIRQData = (statusFlag | TOUCH_DETECTED_IRQ_STATUS_BIT); // Write one to clear bit
	WriteDataToTouchModule(STMPE811_INT_STA, clearIRQData);
	
	uint8_t ctrlReg = ReadRegisterFromTouchModule(STMPE811_TSC_CTRL);
	if (ctrlReg & 0x80)
	{
		isTouchDetected = true;
	}

	// Determine if it is pressed or unpressed
	if(isTouchDetected) // Touch has been detected
	{
		printf("\nPressed");
		// May need to do numerous retries?
		DetermineTouchPosition(&StaticTouchData);
		/* Touch valid */
		printf("\nX: %03d\nY: %03d \n", StaticTouchData.x, StaticTouchData.y);
		LCD_Clear(0, LCD_COLOR_RED);

	}else{

		/* Touch not pressed */
		printf("\nNot pressed \n");
		LCD_Clear(0, LCD_COLOR_GREEN);
	}

	STMPE811_Write(STMPE811_FIFO_STA, 0x01);
	STMPE811_Write(STMPE811_FIFO_STA, 0x00);

	// Re-enable IRQs
    WriteDataToTouchModule(STMPE811_INT_EN, currentIRQEnables);
	HAL_EXTI_ClearPending(&LCDTouchIRQ, EXTI_TRIGGER_RISING_FALLING);

	HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	//Potential ERRATA? Clearing IRQ bit again due to an IRQ being triggered DURING the handling of this IRQ..
	WriteDataToTouchModule(STMPE811_INT_STA, clearIRQData);

}
#endif // TOUCH_INTERRUPT_ENABLED
#endif // COMPILE_TOUCH_FUNCTIONS

void EXTI0_IRQHandler(){
	IRQ_DI(EXTI0_IRQ_NUMBER);

	if (ButtonPressed()){
		//printf("Button pressed, currentScreen = %d\n", currentScreen);
		switch(currentScreen){
		case SCREEN_MAIN_MENU:
			currentScreen = SCREEN_GAMEPLAY;
			HAL_LTDC_SetAlpha(&hltdc, 0, MAIN_MENU_LAYER);
			HAL_LTDC_SetAlpha(&hltdc, 0xFF, GAMEPLAY_LAYER);
			HAL_LTDC_Reload(&hltdc, LTDC_RELOAD_IMMEDIATE);

	        StartTimer();
            generateGameGrid();
            addSchedulerEvent(GENERATE_TETRIMINO_EVENT);
			break;

		case SCREEN_GAMEPLAY:
			//PLAY UNTIL LOSE
			//WHEN LOSE = TRUE, ENDSCREEN LAYER
			addSchedulerEvent(ROTATE_CW_EVENT);
			break;

		case SCREEN_GAME_OVER:
            currentScreen = SCREEN_MAIN_MENU;
            HAL_LTDC_SetAlpha(&hltdc, 0, ENDSCREEN_LAYER);
            HAL_LTDC_SetAlpha(&hltdc, 0xFF, MAIN_MENU_LAYER);
            HAL_LTDC_Reload(&hltdc, LTDC_RELOAD_IMMEDIATE);

	        StartTimer();
	        LCD_Clear(0,LCD_COLOR_BLACK);
            generateGameGrid();
            addSchedulerEvent(GENERATE_TETRIMINO_EVENT);
            break;

		}
	IRQ_CPI(EXTI0_IRQ_NUMBER);
	EXTI_CPIB(GPIO_PIN_0);
	IRQ_EI(EXTI0_IRQ_NUMBER);
	}
}

void TIM2_IRQHandler(void) {
	IRQ_DI(TIM2_IRQ_NUMBER);
		if (TIM2->SR & UIF_SET){		//if status register and update interrupt flagged bit is 1///macro for UIF
			TIM2->SR &= ~(UIF_SET);				//uif is 1 in sr now (CLEAR WITH 1)
		}
			IRQ_CPI(TIM2_IRQ_NUMBER);
			IRQ_EI(TIM2_IRQ_NUMBER);
}
