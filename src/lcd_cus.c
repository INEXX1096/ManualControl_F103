#include "lcd_cus.h"
#include "lcd_st7735.h"

void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    uint16_t i, j;
    LCD_SetWindow(xsta, ysta, xend - 1, yend - 1);  // 设置窗口

    for(i = ysta; i < yend; i++)
    {
        for(j = xsta; j < xend; j++)
        {
            LCD_WriteData16(color);
        }
    }
}

void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color){
    LCD_SetWindow(x, y, x, y);
    LCD_WriteData16(color);
}

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	uint16_t t; 
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;

	delta_x = x2 - x1; 
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;

	if(delta_x > 0) incx = 1; 
	else if(delta_x == 0) incx = 0;
	else {
		incx = -1;
		delta_x = -delta_x;
	}

	if(delta_y > 0) incy = 1; 
	else if(delta_y == 0) incy = 0;
	else {
		incy = -1;
		delta_y = -delta_y; 
	}

	distance = (delta_x > delta_y) ? delta_x : delta_y;

	for(t = 0; t <= distance; t++)
	{
		LCD_DrawPoint(uRow, uCol, color);
		xerr += delta_x;
		yerr += delta_y;

		if(xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if(yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}


void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}