#ifndef __OLED_H__
#define __OLED_H__

#include "main.h"
#include "i2c.h"
#include "stm32f1xx_hal_uart.h"
#include "stm32f103xe.h"

#define OLED_1INCH3

extern struct OLED_Config g_oledCfg;

struct OLED_Config {
    I2C_HandleTypeDef *i2cHandle;
    uint8_t i2cAddr;
};

enum OLED_ASCII_FONT_SIZE {
    O_FONT_8X6,
    O_FONT_12X6,
    O_FONT_16X8,
    O_FONT_24X12
};

int  OLED_Init();

void OLED_SetPos(uint8_t X, uint8_t Y);
void OLED_SetCursor(uint8_t page, uint8_t X);
void OLED_Fill(uint8_t data);

void OLED_NewFrame();
void OLED_ClearArea(int16_t X, int16_t Y, uint8_t width, uint8_t height);
void OLED_ShowFrame();
void OLED_SetPixel(int X, int Y);

void OLED_ShowImage(int16_t X, int16_t Y, uint8_t width, uint8_t height, const uint8_t *image);

void OLED_ShowChar(uint8_t X, uint8_t Y, char c);
void OLED_ShowString(uint8_t X, uint8_t Y, const char *str);
void OLED_ShowChinese(uint8_t X, uint8_t Y, const char *Chinese);

void OLED_DrawPoint(uint8_t X, uint8_t Y);
void OLED_DrawLine(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1);
void OLED_DrawRectangle(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled);

void OLED_SetASCIIFont(enum OLED_ASCII_FONT_SIZE font);
void OLED_Printf(uint8_t X, uint8_t Y, char *format, ...);

// TEST
void OLED_Test();

#endif