#include "st7735.h"
#include "stm32f1xx_hal.h" // 以STM32F1 HAL为例

extern SPI_HandleTypeDef hspi1;  // 你用的SPI句柄

// 控制片选/复位/命令数据等GPIO定义
#define LCD_CS_PORT    GPIOA
#define LCD_CS_PIN     GPIO_PIN_4

#define LCD_DC_PORT    GPIOA
#define LCD_DC_PIN     GPIO_PIN_5

#define LCD_RST_PORT   GPIOA
#define LCD_RST_PIN    GPIO_PIN_6

void LCD_IO_Init(void)
{
    // 初始化 SPI 和相关GPIO，这里假设已经在别处初始化好了SPI
    // 你可以初始化控制引脚GPIO为输出
    // 例子：
    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);
}

void LCD_IO_WriteReg(uint8_t Reg)
{
    HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_RESET); // 命令模式
    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET); // 片选使能
    HAL_SPI_Transmit(&hspi1, &Reg, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);   // 片选失能
}

void LCD_IO_WriteMultipleData(uint8_t *pData, uint32_t Size)
{
    HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET); // 数据模式
    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET); // 片选使能
    HAL_SPI_Transmit(&hspi1, pData, Size, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);   // 片选失能
}

void LCD_Delay(uint32_t delay)
{
    HAL_Delay(delay);  // ms级延时
}
