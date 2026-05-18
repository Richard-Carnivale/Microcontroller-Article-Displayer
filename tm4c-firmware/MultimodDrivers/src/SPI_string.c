// Date Created: 2025-09-17
// Date Updated: 2025-09-18
// Spare String Functions (for SPI printing)

#include "../font.h"
#include "../GFX_Library.h"
// i trust that you guys can figure out how to put the font in it's own header file (from the GFX library)
//  and that you can resolve other header dependencies by now on your own
// and that you can put these (the draw char and draw string) functions in a header file to expose to your toplevel

uint8_t textsize1 = 1;
#define SEQUENTIAL_CHAR_SPACE 5

// écrire un string à un endroit précise
void ST7789_DrawStringStatic(const char* string, const uint16_t text_color, const uint8_t x, const uint8_t y) {
  // temp
  uint8_t temp_cursor_x = x;
  uint8_t temp_cursor_y = y;

  while(*string) {
    uint8_t c = (uint8_t)*string++;

    // ignorer lignes nouveaux
    if (c == '\n' || c == '\r') {
        // NEW
        temp_cursor_x = x;
        temp_cursor_y -= 20;
        continue;
        //return;
    }

    ST7789_DrawCharStatic(c, text_color, temp_cursor_x, temp_cursor_y);
    // temp_cursor_x += (1 - text_size) * SEQUENTIAL_CHAR_SPACE + SEQUENTIAL_CHAR_SPACE;
    temp_cursor_x += (SEQUENTIAL_CHAR_SPACE + textsize1 * 1.6 + (0.4 * textsize1));
    //temp_cursor_y += 0;
    if(temp_cursor_x >= X_MAX-15){
        temp_cursor_x = x;
        temp_cursor_y -= 20;
    }
  }
}

void ST7789_DrawCharStatic(const char c, const uint16_t text_color, const uint8_t x, const uint8_t y) {
  uint8_t line;
  for (uint8_t i = 0; i < 5; i++) {
    line = font[c][i];

    for (uint8_t j = 0; j < 8; j++, line >>= 1) {
      if (line & 1) {
        if (textsize1 == 1) {
          ST7789_DrawPixel(x + i, y - j, text_color);
        } else {
          ST7789_DrawRectangle(x + i*textsize1, y - j*textsize1, textsize1, textsize1, text_color);
        }
      }
    }

  }
}
