#ifndef SPI_string_H
#define SPI_string_H


#include <stdint.h>
#include <stdbool.h>

void ST7789_DrawStringStatic(const char* string, const uint16_t text_color, const uint8_t x, const uint8_t y);
void ST7789_DrawCharStatic(const char c, const uint16_t text_color, const uint8_t x, const uint8_t y);


#endif // SPI_string_H
