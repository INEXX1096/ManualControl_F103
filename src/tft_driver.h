#pragma once

#ifndef TFT_DRIVER_H
#define TFT_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

void tft_init(void);
void tft_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p); // 注意这里 color_p 类型

#ifdef __cplusplus
}
#endif

#endif
