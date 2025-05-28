#include "tft_driver.h"
#include "st7735.h"

void tft_init(void){
    LCD_IO_Init();
    st7735_Init();
    st7735_DisplayOn();
}

void tft_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p){
    uint16_t w = area->x2 - area->x1 + 1;
    uint16_t h = area->y2 - area->y1 + 1;

    st7735_SetDisplayWindow(area->x1, area->y1, w, h);
    LCD_IO_WriteMultipleData((uint8_t *)color_p, w * h * sizeof(lv_color_t));

    lv_display_flush_ready(disp);
}


