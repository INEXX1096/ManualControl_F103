#ifndef LCD_ST7735_H
#define LCD_ST7735_H

#include "stm32f1xx_hal.h"

void LCD_Init(void);
void LCD_WriteCommand(uint8_t cmd);
void LCD_WriteData(uint8_t data);
// 你还可以加更多接口函数

#endif
