// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#ifndef LCD_ST_7735_LCD
#define LCD_ST_7735_LCD

#include "string.h"
#include "st7735/lcd_st7735.h"

enum{
  BGRColorBlack = 0x000000,
  BGRColorBlue = 0xFF0000,
  BGRColorGreen = 0x00FF00,
  BGRColorRed = 0x0000FF,
  BGRColorWhite = 0xFFFFFF,
};

typedef enum TextAlignment{
  alined_right,
  alined_center,
  alined_left,
}TextAlignment_t;

void lcd_println(St7735Context *lcd, const char * str, TextAlignment_t alignment, int32_t line);

typedef struct Menu{
  const char * title;
  const char ** items;
  size_t items_count;
  uint32_t color;
  uint32_t background;
  uint32_t selected_color;
} Menu_t;

void lcd_show_menu(St7735Context *lcd, Menu_t *menu);
#endif
