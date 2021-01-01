#include <Arduino.h>

#include "hardware.h"
#include "memory.h"
#include "tftdisplay.h"

#include "config.h"
#include "display.h"
#include "util/tiles_sprites.h"
#include "roms/rom82s123_7f.h"	// colours
#include "roms/rom82s126_4a.h"	// palette

static void get_palette(palette_entry &p, uint8_t index) {
  index <<= 2; // offset into palette ROM (4 bytes per palette)
  p.set_colour(colours[palette[index  ]], 0);
  p.set_colour(colours[palette[index + 1]], 1);
  p.set_colour(colours[palette[index + 2]], 2);
  p.set_colour(colours[palette[index + 3]], 3);
}

void Display::begin() {
  TFTDisplay::begin(BLACK, WHITE, ORIENT, DISPLAY_WIDTH, DISPLAY_HEIGHT);

  // precompute 565 color palette
  palette_entry p;
  for (int i = 0; i < 32; i++) {
    get_palette(p, i);
    for (int j = 0; j < 4; j++) {
      colour &c = p.colours[j];
      _palette565[i][j] = c.get();
    }
  }
}

void Display::draw_tile(uint16_t t, int x, int y) {
  const uint8_t pindex = _tp[t + 0x0400] & 0x1f;
  const uint8_t *cdata = tiles + _tp[t] * 64;
  for (int n = x; n < x + 8; n++)
    for (int m = y; m < y + 8; m++) {
      drawPixel(n, m, _palette565[pindex][pgm_read_byte(cdata)]);
      cdata++;
    }
}

void Display::renderTiles() {
  int x, y;

  // top line
  for (uint16_t a = 0x3c2, x = 27 * 8; a < 0x3de; a++) {
    draw_tile(a, x, 0);
    x -= 8;
  }
  // 2nd top line
  for (uint16_t a = 0x3e2, x = 27 * 8; a < 0x3fe; a++) {
    draw_tile(a, x, 8);
    x -= 8;
  }
  // general area
  for (uint16_t a = 0x40, x = 27, y = 2; a < 0x3c0; a++) {
    draw_tile(a, x << 3, y << 3);
    y++;
    if (y > 33) {
      y = 2;
      x--;
    }
  }
  // 2nd from bottom line
  for (uint16_t a = 0x02, x = 27 * 8; a < 0x1e; a++) {
    draw_tile(a, x, 34 * 8);
    x -= 8;
  }
  // bottom line
  for (uint16_t a = 0x22, x = 27 * 8; a < 0x03e; a++) {
    draw_tile(a, x, 35 * 8);
    x -= 8;
  }
}

void Display::set_sprite(uint16_t off, uint8_t sx, uint8_t sy) {
  int x = DISPLAY_WIDTH - sx + 15;
  int y = DISPLAY_HEIGHT - sy - 16;
  uint8_t pindex = _mem[0x4ff1 + off]  & 0x1f;;

  uint8_t sir = _mem[0x4ff0 + off];
  bool fx = (sir & 0x02), fy = (sir & 0x01);
  const uint8_t *cdata = sprites + 256 * (sir >> 2);
  for (int n = 0; n < 16; n++)
    for (int m = 0; m < 16; m++) {      
      uint16_t c = _palette565[pindex][pgm_read_byte(cdata)];
      if (c) {
        int px = fx ? x + 15 - n : x + n, py = fy ? y + 15 - m : y + m;
        drawPixel (px, py, c); // black is transparent
      }
      cdata++;
    }
}

void Display::renderSprites(IO &io) {
  // 0x5060-0x506f x,y sprite pair in IO memory
  // higher index sprites write on top of lower sprites.
  for (uint8_t i = SPRITE_START + SPRITE_LEN - 2; i > SPRITE_START ; i -= 2) {
    set_sprite(i - SPRITE_START, io[i], io[i + 1]);
  }
}
