#ifndef LCD_ST7735_H
#define LCD_ST7735_H

#include "stm32f1xx_hal.h"
#include <stdint.h>

// PB4-PB9 分别对应你的信号线（根据你的硬件连接调整）
#define LCD_SCLK_GPIO_Port GPIOB
#define LCD_SCLK_Pin       GPIO_PIN_4

#define LCD_MOSI_GPIO_Port GPIOB
#define LCD_MOSI_Pin       GPIO_PIN_5

#define LCD_CS_GPIO_Port   GPIOB
#define LCD_CS_Pin         GPIO_PIN_6

#define LCD_DC_GPIO_Port   GPIOB
#define LCD_DC_Pin         GPIO_PIN_7

#define LCD_RST_GPIO_Port  GPIOB
#define LCD_RST_Pin        GPIO_PIN_8

#define LCD_BL_GPIO_Port   GPIOB
#define LCD_BL_Pin         GPIO_PIN_9

// 控制片选的宏，根据你的定义调整
#define LCD_CS_Select()   HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
#define LCD_CS_Unselect() HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)

// 时钟和数据线控制
#define LCD_SCLK_H() HAL_GPIO_WritePin(LCD_SCLK_GPIO_Port, LCD_SCLK_Pin, GPIO_PIN_SET)
#define LCD_SCLK_L() HAL_GPIO_WritePin(LCD_SCLK_GPIO_Port, LCD_SCLK_Pin, GPIO_PIN_RESET)
#define LCD_MOSI_H() HAL_GPIO_WritePin(LCD_MOSI_GPIO_Port, LCD_MOSI_Pin, GPIO_PIN_SET)
#define LCD_MOSI_L() HAL_GPIO_WritePin(LCD_MOSI_GPIO_Port, LCD_MOSI_Pin, GPIO_PIN_RESET)

// 初始化
void LCD_Init(void);
void LCD_GPIO_Init(void);
void LCD_Reset(void);

// 基本写入函数
void LCD_WriteCommand(uint8_t cmd);
void LCD_WriteData(uint8_t data);
void LCD_WriteData16(uint16_t data);
void LCD_SPI_WriteByte(uint8_t data);

// 显示窗口设置
void LCD_SetWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

// 屏幕操作
void LCD_FillScreen(uint16_t color);
void LCD_DrawPixel(uint8_t x, uint8_t y, uint16_t color);  // 可选加一个单点绘制

#endif
