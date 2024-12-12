/*
 * LCD_Driver.c
 *
 *  Created on: Sep 28, 2023
 *      Author: Xavion
 */

#include "LCD_Driver.h"
#include "GamePlay.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_ltdc.h"

/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */

static LTDC_HandleTypeDef hltdc;
static RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
static FONT_t *LCD_Currentfonts;
static uint16_t CurrentTextColor = LCD_COLOR_BLACK;





/*
 * fb[y*W+x] OR fb[y][x]
 * Alternatively, we can modify the linker script to have an end address of 20013DFB instead of 2002FFFF, so it does not place variables in the same region as the frame buffer. In this case it is safe to just specify the raw address as frame buffer.
 */
//uint32_t frameBuffer[(LCD_PIXEL_WIDTH*LCD_PIXEL_WIDTH)/2] = {0};		//16bpp pixel format. We can size to uint32. this ensures 32 bit alignment


//Someone from STM said it was "often accessed" a 1-dim array, and not a 2d array. However you still access it like a 2dim array,  using fb[y*W+x] instead of fb[y][x].
uint16_t frameBuffer[LCD_PIXEL_WIDTH*LCD_PIXEL_HEIGHT] = {0};			//16bpp pixel format.



void LCD_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable the LTDC clock */
  __HAL_RCC_LTDC_CLK_ENABLE();

  /* Enable GPIO clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /* GPIO Config
   *
    LCD pins
   LCD_TFT R2 <-> PC.10
   LCD_TFT G2 <-> PA.06
   LCD_TFT B2 <-> PD.06
   LCD_TFT R3 <-> PB.00
   LCD_TFT G3 <-> PG.10
   LCD_TFT B3 <-> PG.11
   LCD_TFT R4 <-> PA.11
   LCD_TFT G4 <-> PB.10
   LCD_TFT B4 <-> PG.12
   LCD_TFT R5 <-> PA.12
   LCD_TFT G5 <-> PB.11
   LCD_TFT B5 <-> PA.03
   LCD_TFT R6 <-> PB.01
   LCD_TFT G6 <-> PC.07
   LCD_TFT B6 <-> PB.08
   LCD_TFT R7 <-> PG.06
   LCD_TFT G7 <-> PD.03
   LCD_TFT B7 <-> PB.09
   LCD_TFT HSYNC <-> PC.06
   LCDTFT VSYNC <->  PA.04
   LCD_TFT CLK   <-> PG.07
   LCD_TFT DE   <->  PF.10
  */

  /* GPIOA configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_11 | GPIO_PIN_12;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
  GPIO_InitStructure.Alternate= GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

 /* GPIOB configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

 /* GPIOC configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

 /* GPIOD configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_3 | GPIO_PIN_6;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

 /* GPIOF configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_10;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);

 /* GPIOG configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | \
                           GPIO_PIN_11;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

  /* GPIOB configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStructure.Alternate= GPIO_AF9_LTDC;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* GPIOG configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_10 | GPIO_PIN_12;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);  // Adjust IRQ number and priority
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void LTCD_Layer_Init(uint8_t LayerIndex)
{
	LTDC_LayerCfgTypeDef  pLayerCfg;

	pLayerCfg.WindowX0 = 0;	//Configures the Window HORZ START Position.
	pLayerCfg.WindowX1 = LCD_PIXEL_WIDTH;	//Configures the Window HORZ Stop Position.
	pLayerCfg.WindowY0 = 0;	//Configures the Window vertical START Position.
	pLayerCfg.WindowY1 = LCD_PIXEL_HEIGHT;	//Configures the Window vertical Stop Position.
	pLayerCfg.PixelFormat = LCD_PIXEL_FORMAT_1;  //INCORRECT PIXEL FORMAT WILL GIVE WEIRD RESULTS!! IT MAY STILL WORK FOR 1/2 THE DISPLAY!!! //This is our buffers pixel format. 2 bytes for each pixel
	pLayerCfg.Alpha = 255;
	pLayerCfg.Alpha0 = 0;
	pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
	pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
	if (LayerIndex == 0){
		pLayerCfg.FBStartAdress = (uintptr_t)frameBuffer;
	}
	pLayerCfg.ImageWidth = LCD_PIXEL_WIDTH;
	pLayerCfg.ImageHeight = LCD_PIXEL_HEIGHT;
	pLayerCfg.Backcolor.Blue = 0;
	pLayerCfg.Backcolor.Green = 0;
	pLayerCfg.Backcolor.Red = 0;
	if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, LayerIndex) != HAL_OK)
	{
		LCD_Error_Handler();
	}
}

void clearScreen(void)
{
  LCD_Clear(0,LCD_COLOR_WHITE);
}

void LTCD_Init(void)
{
	hltdc.Instance = LTDC;

	/* Configure horizontal synchronization width */
	hltdc.Init.HorizontalSync = ILI9341_HSYNC;
	/* Configure vertical synchronization height */
	hltdc.Init.VerticalSync = ILI9341_VSYNC;
	/* Configure accumulated horizontal back porch */
	hltdc.Init.AccumulatedHBP = ILI9341_HBP;
	/* Configure accumulated vertical back porch */
	hltdc.Init.AccumulatedVBP = ILI9341_VBP;
	/* Configure accumulated active width */
	hltdc.Init.AccumulatedActiveW = 269;
	/* Configure accumulated active height */
	hltdc.Init.AccumulatedActiveH = 323;
	/* Configure total width */
	hltdc.Init.TotalWidth = 279;
	/* Configure total height */
	hltdc.Init.TotalHeigh = 327;
	/* Configure R,G,B component values for LCD background color */
	hltdc.Init.Backcolor.Red = 0;
	hltdc.Init.Backcolor.Blue = 0;
	hltdc.Init.Backcolor.Green = 0;

	/* LCD clock configuration */
	/* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
	/* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
	/* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/4 = 48 Mhz */
	/* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_8 = 48/4 = 6Mhz */

	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
	PeriphClkInitStruct.PLLSAI.PLLSAIR = 4;
	PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
	/* Polarity */
	hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
	hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
	hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
	hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

	LCD_GPIO_Init();

	if (HAL_LTDC_Init(&hltdc) != HAL_OK)
	 {
	   LCD_Error_Handler();
	 }

	ili9341_Init();
}

/* START Draw functions */


/*
 * This is really the only function needed.
 * All drawing consists of is manipulating the array.
 * Adding input sanitation should probably be done.
 */
void LCD_Draw_Pixel(uint16_t x, uint16_t y, uint16_t color)
{
	if(x < 0 || x > 239 || y < 0 || y > 319){
		return;
	}
	frameBuffer[y*LCD_PIXEL_WIDTH+x] = color;  //You cannot do x*y to set the pixel.
}

/*
 * These functions are simple examples. Most computer graphics like OpenGl and stm's graphics library use a state machine. Where you first call some function like SetColor(color), SetPosition(x,y), then DrawSqure(size)
 * Instead all of these are explicit where color, size, and position are passed in.
 * There is tons of ways to handle drawing. I dont think it matters too much.
 */
void LCD_Draw_Circle_Fill(uint16_t Xpos, uint16_t Ypos, uint16_t radius, uint16_t color)
{
    for(int16_t y=-radius; y<=radius; y++)
    {
        for(int16_t x=-radius; x<=radius; x++)
        {
            if(x*x+y*y <= radius*radius)
            {
            	LCD_Draw_Pixel(x+Xpos, y+Ypos, color);
            }
        }
    }
}

void LCD_Draw_Rectangle_Fill(uint16_t Xpos, uint16_t Ypos, uint16_t width, uint16_t height, uint16_t color){
	for (int16_t y = 0; y < height; y++){
		for (int16_t x = 0; x < width; x++){
	            LCD_Draw_Pixel(Xpos+x, Ypos+y, color);
	    }
	}
}

void LCD_Draw_Vertical_Line(uint16_t x, uint16_t y, uint16_t len, uint16_t color)
{
  for (uint16_t i = 0; i < len; i++)
  {
	  LCD_Draw_Pixel(x, i+y, color);
  }
}

void LCD_Draw_Horizontal_Line(uint16_t x, uint16_t y, uint16_t len, uint16_t color)
{
  for (uint16_t i = 0; i < len; i++)
  {
	  LCD_Draw_Pixel(i+x, y, color);
  }
}
void LCD_DrawLayer(uint8_t LayerIndex){
	if (LayerIndex == MAIN_MENU_LAYER){
			DrawMainMenu();
		}

	if (LayerIndex == GAMEPLAY_LAYER){
			LCD_Clear(0,LCD_COLOR_BLACK);
			LCD_DrawGrid();
		}

	if (LayerIndex == ENDSCREEN_LAYER){
			drawEndScreen();
		}
}

void DrawTetrimino(uint16_t Xpos, uint16_t Ypos, uint8_t tetrimino[4][4], uint16_t color){
	uint16_t x, y;
	uint16_t i, j;
	uint16_t blockSize = 20;

	for(y = 0; y < 4; y++){
		for(x = 0; x < 4; x++){
			if (tetrimino[y][x] == 1) {
				uint16_t pixelX = Xpos + x * blockSize;
				uint16_t pixelY = Ypos + y * blockSize; //////////////
				for (i = 0; i < blockSize; i++) {
				   for (j = 0; j < blockSize; j++) {
				       LCD_Draw_Pixel(pixelX + i, pixelY + j, color); //fill in block, resolve error of single pixel every 20x pixels
				   }
				}
			}
		}
	}
}

void MainTetriminoes(void){
	DrawTetriminos(0, 50, 30);
	DrawTetriminos(1, 120, 30);
	DrawTetriminos(2, 10, 80);
	DrawTetriminos(3, 150, 130);
	DrawTetriminos(4, 100, 110);
	DrawTetriminos(5, 30, 170);
	DrawTetriminos(6, 130, 180);
}

uint16_t TetriminoColors(int x){
	switch(x){
		case 0:
			return LCD_COLOR_CYAN;
			break;
		case 1:
			return LCD_COLOR_YELLOW;
			break;
		case 2:
			return LCD_COLOR_MAGENTA;
			break;
		case 3:
			return LCD_COLOR_ORANGE;
			break;
		case 4:
			return LCD_COLOR_BLUE;
			break;
		case 5:
			return LCD_COLOR_GREEN;
			break;
		case 6:
			return LCD_COLOR_RED;
			break;
		default:
			return 0;
	}
}


void DrawTetriminos(int x, uint16_t posX, uint16_t posY){
	switch(x){
	case 0:
		DrawTetrimino(posX, posY, Tetriminos[0], TetriminoColors(0));
		break;
	case 1:
		DrawTetrimino(posX, posY, Tetriminos[1], TetriminoColors(1));
		break;
	case 2:
		DrawTetrimino(posX, posY, Tetriminos[2], TetriminoColors(2));
		break;
	case 3:
		DrawTetrimino(posX, posY, Tetriminos[3], TetriminoColors(3));
		break;
	case 4:
		DrawTetrimino(posX, posY, Tetriminos[4], TetriminoColors(4));
		break;
	case 5:
		DrawTetrimino(posX, posY, Tetriminos[5], TetriminoColors(5));
		break;
	case 6:
		DrawTetrimino( posX, posY, Tetriminos[6], TetriminoColors(6));
		break;
	}
}

void LCD_Clear(uint8_t LayerIndex, uint16_t Color)
{
	if (LayerIndex == 0){
		for (uint32_t i = 0; i < LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT; i++){
			frameBuffer[i] = Color;
		}
	}
}

//This was taken and adapted from stm32's mcu code
void LCD_SetTextColor(uint16_t Color)
{
  CurrentTextColor = Color;
}

//This was taken and adapted from stm32's mcu code
void LCD_SetFont(FONT_t *fonts)
{
  LCD_Currentfonts = fonts;
}

//This was taken and adapted from stm32's mcu code
void LCD_Draw_Char(uint16_t Xpos, uint16_t Ypos, const uint16_t *c)
{
  uint32_t index = 0, counter = 0;
  for(index = 0; index < LCD_Currentfonts->Height; index++)
  {
    for(counter = 0; counter < LCD_Currentfonts->Width; counter++)
    {
      if((((c[index] & ((0x80 << ((LCD_Currentfonts->Width / 12 ) * 8 ) ) >> counter)) == 0x00) && (LCD_Currentfonts->Width <= 12)) || (((c[index] & (0x1 << counter)) == 0x00)&&(LCD_Currentfonts->Width > 12 )))
      {
         //Background If want to overrite text under then add a set color here
      }
      else
      {
    	  LCD_Draw_Pixel(counter + Xpos,index + Ypos,CurrentTextColor);
      }
    }
  }
}

//This was taken and adapted from stm32's mcu code
void LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii)
{
  Ascii -= 32;
  LCD_Draw_Char(Xpos, Ypos, &LCD_Currentfonts->table[Ascii * LCD_Currentfonts->Height]);
}

void LCD_DisplayInt(uint16_t Xpos, uint16_t Ypos, uint32_t rnum){
	char buffer[10];
	int i = 0;

	if (rnum == 0){
		LCD_DisplayChar(Xpos, Ypos, '0');
		return;
	}
	while (rnum > 0) {					//extract in reverse order
		buffer[i] = (rnum % 10) + '0';
		rnum /= 10;						//shifts digits one place to the right
	    i++;
	}

	for (int j = i - 1; j >= 0; j--) {
		LCD_DisplayChar(Xpos, Ypos, buffer[j]);
	    Xpos += 15;
	}
}

void LCD_DrawGrid(void){
	uint16_t x, y;

	for (y = 0; y <= GRID_HEIGHT; y++){
	   uint16_t startY = y * BLOCK_SIZE;
	   LCD_Draw_Horizontal_Line(0, startY, GRID_WIDTH * BLOCK_SIZE, LCD_COLOR_WHITE);
	}
	for (x = 0; x <= GRID_WIDTH; x++) {
		uint16_t startX = x * BLOCK_SIZE;
	    LCD_Draw_Vertical_Line(startX, 0, GRID_HEIGHT * BLOCK_SIZE, LCD_COLOR_WHITE);
	}
}






//void visualDemo(void)
//{
	//uint16_t x;
	//uint16_t y;
	// This for loop just illustrates how with using logic and for loops, you can create interesting things
	// this may or not be useful ;)
//	for(y=0; y<LCD_PIXEL_HEIGHT; y++){
//		for(x=0; x < LCD_PIXEL_WIDTH; x++){
//			if (x & 32)
//				frameBuffer[x*y] = LCD_COLOR_WHITE;
//			else
//				frameBuffer[x*y] = LCD_COLOR_BLACK;
//		}
//	}

//	LCD_Clear(0,LCD_COLOR_PINK);
//
//	LCD_SetTextColor(LCD_COLOR_BLACK);
//	LCD_SetFont(&Font16x24);

	//TESTING FOR RNG - COMPLETE
	//char randomchar = RNGAscii();
	//LCD_Draw_Char(100,50,randomchar);
//}

void LCD_ScreenLayers(){
	LTCD_Layer_Init(MAIN_MENU_LAYER);
	LTCD_Layer_Init(GAMEPLAY_LAYER);
	LTCD_Layer_Init(ENDSCREEN_LAYER);

	HAL_LTDC_SetAlpha(&hltdc, 0xFF, MAIN_MENU_LAYER); 		//225 implies visible. this is the initial state
	HAL_LTDC_SetAlpha(&hltdc, 0x00, GAMEPLAY_LAYER);   			 //0 is NOT visible
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */

void LCD_Error_Handler(void){
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  	  {
  	  }
 }

  /* USER CODE END Error_Handler_Debug */


// Touch Functionality   //

#if COMPILE_TOUCH_FUNCTIONS == 1

void InitializeLCDTouch(void)
{
  if(STMPE811_Init() != STMPE811_State_Ok)
  {
	 for(;;); // Hang code due to error in initialzation
  }
}

STMPE811_State_t returnTouchStateAndLocation(STMPE811_TouchData * touchStruct)
{
	return STMPE811_ReadTouch(touchStruct);
}

void DetermineTouchPosition(STMPE811_TouchData * touchStruct)
{
	STMPE811_DetermineTouchPosition(touchStruct);
}

uint8_t ReadRegisterFromTouchModule(uint8_t RegToRead)
{
	return STMPE811_Read(RegToRead);
}

void WriteDataToTouchModule(uint8_t RegToWrite, uint8_t writeData)
{
	STMPE811_Write(RegToWrite, writeData);
}

#endif // COMPILE_TOUCH_FUNCTIONS
