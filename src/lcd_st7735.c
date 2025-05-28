#include "lcd_st7735.h"

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

// GPIO 初始化
void LCD_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = LCD_SCLK_Pin | LCD_MOSI_Pin | LCD_CS_Pin | LCD_DC_Pin | LCD_RST_Pin | LCD_BL_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 初始化默认状态
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);   // 片选高（不选中）
    HAL_GPIO_WritePin(LCD_SCLK_GPIO_Port, LCD_SCLK_Pin, GPIO_PIN_RESET); // 时钟初始低
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);   // 默认数据模式
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET); // 复位高，非复位状态
    HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);   // 背光打开（根据硬件决定高低）
}


void LCD_SPI_WriteByte(uint8_t data)
{
    for (int8_t i = 7; i >= 0; i--)
    {
        // 先拉低时钟
        HAL_GPIO_WritePin(LCD_SCLK_GPIO_Port, LCD_SCLK_Pin, GPIO_PIN_RESET);

        // 设置数据线
        if (data & (1 << i))
            HAL_GPIO_WritePin(LCD_MOSI_GPIO_Port, LCD_MOSI_Pin, GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(LCD_MOSI_GPIO_Port, LCD_MOSI_Pin, GPIO_PIN_RESET);

        // 拉高时钟，数据被 LCD 采样
        HAL_GPIO_WritePin(LCD_SCLK_GPIO_Port, LCD_SCLK_Pin, GPIO_PIN_SET);
    }
}


void LCD_WriteCommand(uint8_t cmd)
{
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET); // DC=0 命令
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // 片选使能
    LCD_SPI_WriteByte(cmd);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);   // 片选失能
}

void LCD_WriteData(uint8_t data)
{
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); // DC=1 数据
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // 片选使能
    LCD_SPI_WriteByte(data);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);   // 片选失能
}


void LCD_Reset(void)
{
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(120);
}

void LCD_Init(void)
{
    LCD_GPIO_Init();
    LCD_Reset();

    // 这里简单示范几个初始化命令，完整可参照你的初始化代码补全
    LCD_WriteCommand(0x01);  // 软件复位
    HAL_Delay(150);

    LCD_WriteCommand(0x11);  // 退出睡眠模式
    HAL_Delay(150);

    // 设置颜色模式，16bit
    LCD_WriteCommand(0x3A);
    LCD_WriteData(0x05);

    // 开显示
    LCD_WriteCommand(0x29);
}


void LCD_SetWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    LCD_WriteCommand(0x2A); // Column addr set
    LCD_WriteData(x0);
    LCD_WriteData(x1);

    LCD_WriteCommand(0x2B); // Row addr set
    LCD_WriteData(y0);
    LCD_WriteData(y1);

    LCD_WriteCommand(0x2C); // Memory write
}

void LCD_FillScreen(uint16_t color)
{
    uint16_t i;
    LCD_SetWindow(0, 0, 127, 159);  // ST7735 128x160分辨率

    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); // 数据模式
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // 片选使能

    for(i = 0; i < 128 * 160; i++)
    {
        // 发送高8位
        LCD_SPI_WriteByte(color >> 8);
        // 发送低8位
        LCD_SPI_WriteByte(color & 0xFF);
    }

    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // 片选失能
}
