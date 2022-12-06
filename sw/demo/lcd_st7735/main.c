// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include "demo_system.h"
#include "timer.h"
#include "gpio.h"
#include "spi.h"
#include "st7735/lcd_st7735.h"
#include "core/lucida_console_10pt.h"
#include "lowrisc_logo.h"
#include "lcd.h"

enum{
  LcdSclkPin = 0,
  LcdMosiPin,
  LcdDcPin = 0,
  LcdCsPin ,
  LcdRstPin,
  LcdBlPin,
  SpiSpeedHz = 5 * 100 * 1000,
};


static uint32_t spi_write(void *handle, uint8_t *data, size_t len);
static uint32_t gpio_write(void *handle, bool cs, bool dc);
static void timer_delay(uint32_t ms);
static void timer_delay_micro(uint64_t micro);

int main(void) {
  spi_t spi;

  set_output_bit(GPIO0, LcdDcPin, 0x0);
  set_output_bit(GPIO0, LcdBlPin, 0x1);
  set_output_bit(GPIO0, LcdCsPin, 0x0);

  spi_init(&spi, DEFAULT_SPI, SpiSpeedHz);
  spi.gpio = GPIO0;
  spi.mosi_pin = LcdMosiPin;
  spi.sclk_pin = LcdSclkPin;
  // timer_init();
  // timer_enable(70000);

  St7735Context lcd;
  LCD_Interface interface = {
      .handle = &spi,
      .spi_write = spi_write,
      .gpio_write = gpio_write,
      .timer_delay = timer_delay,
  };
  set_output_bit(GPIO0, LcdRstPin, 0x0);
  timer_delay(150);
  set_output_bit(GPIO0, LcdRstPin, 0x1);

  lcd_st7735_init(&lcd, &interface);
  lcd_st7735_set_font(&lcd, &lucidaConsole_10ptFont);
  lcd_st7735_set_font_colors(&lcd, BGRColorWhite, BGRColorBlack);

  do {
    lcd_st7735_fill_rectangle(&lcd, (LCD_rectangle){.origin = {.x = 0, .y = 0},
        .width = 160, .height = 128}, BGRColorWhite);

    // Draw an image.
    lcd_st7735_draw_rgb565(&lcd, (LCD_rectangle){.origin = {.x = (160 - 105)/2, .y = 5},
                                 .width = 105, .height = 80}, (uint8_t*)lowrisc_logo_105x80);
    lcd_println(&lcd, "Booting...", alined_center, 7);
    timer_delay(2000); 

    lcd_st7735_fill_rectangle(&lcd, (LCD_rectangle){.origin = {.x = 0, .y = 0},
        .width = 160, .height = 128}, BGRColorWhite);
        
    const char * items[] = {"0. LED","1. Fractal","2. PWM",};
    Menu_t main_menu = {
      .title = "Main menu",
      .color = BGRColorBlue,
      .selected_color = BGRColorRed,
      .background = BGRColorWhite,
      .items_count = sizeof(items)/sizeof(items[0]),
      .items = items,
    };

    lcd_show_menu(&lcd, &main_menu);
    timer_delay(10000); 
  } while(1);
}

static uint32_t spi_write(void *handle, uint8_t *data, size_t len){
    while(len--){
      spi_send_byte_blocking(handle, *data++);
    }
    while((spi_get_status(handle) & spi_status_fifo_empty) != spi_status_fifo_empty);
    return 0;
}

static uint32_t gpio_write(void *handle, bool cs, bool dc){
  set_output_bit(GPIO0, LcdDcPin, dc);
  set_output_bit(GPIO0, LcdCsPin, cs);
  return 0;
}

static void timer_delay(uint32_t ms){
  while(ms--){
    timer_delay_micro(1000);
  }
  return;
  uint32_t timeout = get_elapsed_time() + ms;
  while(get_elapsed_time() < timeout){}
}

static void timer_delay_micro(uint64_t micro){
  while(micro--){
    for(uint32_t j = 0; j < 2; ++j){
    }
  }
}
