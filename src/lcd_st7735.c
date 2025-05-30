#include "lcd_st7735.h"
#include "main.h"   // 包含 HAL_GPIO_WritePin 等定义



// GPIO初始化
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
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_SCLK_GPIO_Port, LCD_SCLK_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
}

static void SPI_Delay(void)
{
    for (volatile int i = 0; i < 10; i++)__NOP();
}

void LCD_SPI_WriteByte(uint8_t data)
{
    LCD_CS_Select();      // 片选拉低，开始传输

    for (int8_t i = 7; i >= 0; i--)
    {
        LCD_SCLK_L();     // 时钟拉低

        if (data & (1 << i))
            LCD_MOSI_H();
        else
            LCD_MOSI_L();

        SPI_Delay();

        LCD_SCLK_H();     // 时钟拉高，LCD 读取数据

        SPI_Delay();
    }

    LCD_CS_Unselect();    // 片选拉高，传输结束
}

void LCD_WriteCommand(uint8_t cmd)
{
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET); // DC=0，命令
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // 片选使能
    LCD_SPI_WriteByte(cmd);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);   // 片选失能
}

void LCD_WriteData(uint8_t data)
{
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); // DC=1，数据
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
    LCD_SPI_WriteByte(data);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}
void LCD_WriteData16(uint16_t data)
{
    LCD_WriteData(data >> 8);     // 高8位
    LCD_WriteData(data & 0xFF);   // 低8位
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

    // 复位
    LCD_Reset();
    HAL_Delay(100);

    // 打开背光
    HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
    HAL_Delay(100);

    // 初始化命令序列 (跟你的寄存器库版本一致)
    LCD_WriteCommand(0x11); // Sleep out
    HAL_Delay(120);

    //----------------------------------------------
    LCD_WriteCommand(0xB1);
    LCD_WriteData(0x02);
    LCD_WriteData(0x35);
    LCD_WriteData(0x36);

    LCD_WriteCommand(0xB2);
    LCD_WriteData(0x02);
    LCD_WriteData(0x35);
    LCD_WriteData(0x36);

    LCD_WriteCommand(0xB3);
    LCD_WriteData(0x02);
    LCD_WriteData(0x35);
    LCD_WriteData(0x36);
    LCD_WriteData(0x02);
    LCD_WriteData(0x35);
    LCD_WriteData(0x36);

    LCD_WriteCommand(0xB4); // Dot inversion
    LCD_WriteData(0x03);

    LCD_WriteCommand(0xC0);
    LCD_WriteData(0xA2);
    LCD_WriteData(0x02);
    LCD_WriteData(0x84);

    LCD_WriteCommand(0xC1);
    LCD_WriteData(0xC5);

    LCD_WriteCommand(0xC2);
    LCD_WriteData(0x0D);
    LCD_WriteData(0x00);

    LCD_WriteCommand(0xC3);
    LCD_WriteData(0x8D);
    LCD_WriteData(0x2A);

    LCD_WriteCommand(0xC4);
    LCD_WriteData(0x8D);
    LCD_WriteData(0xEE);

    LCD_WriteCommand(0xC5);
    LCD_WriteData(0x0a);

    LCD_WriteCommand(0x36);
    #if USE_HORIZONTAL == 0
    LCD_WriteData(0x08);
    #elif USE_HORIZONTAL == 1
    LCD_WriteData(0xC8);
    #elif USE_HORIZONTAL == 2
    LCD_WriteData(0x78);
    #else
    LCD_WriteData(0xA8);
    #endif

    LCD_WriteCommand(0xE0);
    LCD_WriteData(0x12);
    LCD_WriteData(0x1C);
    LCD_WriteData(0x10);
    LCD_WriteData(0x18);
    LCD_WriteData(0x33);
    LCD_WriteData(0x2C);
    LCD_WriteData(0x25);
    LCD_WriteData(0x28);
    LCD_WriteData(0x28);
    LCD_WriteData(0x27);
    LCD_WriteData(0x2F);
    LCD_WriteData(0x3C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x03);
    LCD_WriteData(0x03);
    LCD_WriteData(0x10);

    LCD_WriteCommand(0xE1);
    LCD_WriteData(0x12);
    LCD_WriteData(0x1C);
    LCD_WriteData(0x10);
    LCD_WriteData(0x18);
    LCD_WriteData(0x2D);
    LCD_WriteData(0x28);
    LCD_WriteData(0x23);
    LCD_WriteData(0x28);
    LCD_WriteData(0x28);
    LCD_WriteData(0x26);
    LCD_WriteData(0x2F);
    LCD_WriteData(0x3B);
    LCD_WriteData(0x00);
    LCD_WriteData(0x03);
    LCD_WriteData(0x03);
    LCD_WriteData(0x10);

    LCD_WriteCommand(0x3A);
    LCD_WriteData(0x05); // 16-bit color mode

    LCD_WriteCommand(0x29); // Display on
}

void LCD_SetWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    // 偏移修正量（不同方向定义偏移量）
#if USE_HORIZONTAL == 0
    x0 += 2; x1 += 2;
    y0 += 1; y1 += 1;
#elif USE_HORIZONTAL == 1
    x0 += 2; x1 += 2;
    y0 += 3; y1 += 3;
#elif USE_HORIZONTAL == 2
    x0 += 1; x1 += 1;
    y0 += 2; y1 += 2;
#elif USE_HORIZONTAL == 3
    x0 += 3; x1 += 3;
    y0 += 2; y1 += 2;
#endif

    // 设置列地址
    LCD_WriteCommand(0x2A);  // CASET (Column Address Set)
    LCD_WriteData(x0);
    LCD_WriteData(x1);

    // 设置行地址
    LCD_WriteCommand(0x2B);  // RASET (Row Address Set)
    LCD_WriteData(y0);
    LCD_WriteData(y1);

    // 写入内存准备
    LCD_WriteCommand(0x2C);  // RAMWR (Memory Write)
}
void LCD_FillScreen(uint16_t color)
{
    uint32_t i;
    LCD_SetWindow(0, 0, 127, 159);

    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);

    for(i = 0; i < 128 * 160; i++)
    {
        LCD_SPI_WriteByte(color >> 8);
        LCD_SPI_WriteByte(color & 0xFF);
    }

    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

void LCD_DrawPixel(uint8_t x, uint8_t y, uint16_t color)
{
    LCD_SetWindow(x, y, x, y);

    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);

    LCD_SPI_WriteByte(color >> 8);
    LCD_SPI_WriteByte(color & 0xFF);

    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}
