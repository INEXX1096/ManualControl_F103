#include "tft_driver.h"
#include "lcd_st7735.h"  // 你自己写的 GPIO 模拟 SPI 驱动

void tft_init(void)
{
    LCD_Init();  // 初始化 GPIO、重置 LCD、基础寄存器配置
}

void tft_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p)
{
    uint16_t w = area->x2 - area->x1 + 1;
    uint16_t h = area->y2 - area->y1 + 1;

    LCD_SetWindow(area->x1, area->y1, area->x2, area->y2);

    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); // 数据模式
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // 片选使能

    for (uint32_t i = 0; i < w * h; i++)
    {
        // 每个像素是 2 字节（RGB565）
        uint16_t color = (color_p[2 * i] << 8) | color_p[2 * i + 1];
        LCD_SPI_WriteByte(color >> 8);
        LCD_SPI_WriteByte(color & 0xFF);
    }

    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);   // 片选失能
    lv_display_flush_ready(disp); // 通知LVGL刷新完成
}
