#ifndef __OLED_H_
#define __OLED_H_

#include "main.h"

extern I2C_HandleTypeDef hi2c1;

void oled_full(uint8_t data);
void oled_init(void);
void oled_display_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size);
void oled_show_string(uint8_t x, uint8_t y, char ch[], uint8_t TextSize);
void oled_clear(void);
void oled_display_ok_flag(uint8_t x, uint8_t y);

#endif
