// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include "string.h"
#include "lcd.h"
#include "st7735/lcd_st7735.h"

void lcd_show_menu(St7735Context *lcd, Menu_t *menu){
    size_t line = 0;
    size_t selected = 1;
    lcd_st7735_fill_rectangle(lcd, (LCD_rectangle){.origin = {.x = 0, .y = 0},
      .width = lcd->parent.width, .height = lcd->parent.font->height}, menu->color);


    lcd_st7735_set_font_colors(lcd, menu->color, menu->background);
    lcd_println(lcd, menu->title, alined_center, line++);
    lcd_st7735_set_font_colors(lcd, menu->background, menu->color);
    for (int i = 0; i < menu->items_count; ++i){
    //   if (selected == i){
    //     lcd_st7735_set_font_colors(lcd, menu->selected_color, menu->background);
    //   }
      lcd_println(lcd, menu->items[i], alined_left, line++);
    }

    selected++;
    lcd_st7735_draw_horizontal_line(lcd, (LCD_Line) {{.x = 0, .y = lcd->parent.font->height * selected}, lcd->parent.width}, menu->selected_color);
    lcd_st7735_draw_horizontal_line(lcd, (LCD_Line) {{.x = 0, .y = lcd->parent.font->height * (selected + 1) - 1}, lcd->parent.width}, menu->selected_color);
    lcd_st7735_draw_vertical_line(lcd, (LCD_Line) {{.x = 0, .y = lcd->parent.font->height * selected}, lcd->parent.font->height -1}, menu->selected_color);
    lcd_st7735_draw_vertical_line(lcd, (LCD_Line) {{.x = lcd->parent.width - 1, .y = lcd->parent.font->height * selected}, lcd->parent.font->height-1}, menu->selected_color);
}

void lcd_println(St7735Context *lcd, const char * str, TextAlignment_t alignment, int32_t line){
  LCD_Point pos = {
    .y = line * lcd->parent.font->height,
    .x = 0
  };

  if (alignment != alined_left) {
    pos.x = lcd->parent.width - strlen(str) * lcd->parent.font->descriptor_table->width;
    if (alignment == alined_center) {
        pos.x /= 2;
    }
  }

  lcd_st7735_puts(lcd, pos, str); 
}
