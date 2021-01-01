#include <stdint.h>
#include "hardware.h"
#include "memory.h"
#include "tftdisplay.h"

#include <TFT_eSPI.h>

static TFT_eSPI tft;
static TFT_eSprite espi = TFT_eSprite(&tft); // used as a framebuffer

static inline void setColor(colour_t c) {
	espi.setTextColor(c);
}

void TFTDisplay::begin(unsigned bg, unsigned fg, orientation_t orient, unsigned w, unsigned h) {
	_bg = bg;
	_fg = fg;

	tft.init();
	tft.setRotation(orient);
	tft.fillScreen(bg);
	_dx = tft.width();
	_dy = tft.height();
	_cy = tft.fontHeight();
	_cx = 6;	// FIXME

	_xoff = (_dx - w) / 2;
	_yoff = (_dy - h) / 2;

	setColor(fg);
	_oxs = _dx;
	espi.setColorDepth(8);
	espi.createSprite(w,h); // framebuffer

}

void TFTDisplay::clear() {
	espi.fillScreen(_bg);
}

void TFTDisplay::status(const char *s) {
	setColor(_fg);

	espi.fillRect(_dx - _oxs, _dy - _cy, _oxs, _cy, _bg);
	_oxs = espi.textWidth(s);
	espi.setTextDatum(BR_DATUM);
	espi.drawString(s, _dx, _dy);
}

void TFTDisplay::drawPixel(unsigned x, unsigned y, colour_t col) {
	espi.drawPixel(x, y, col);
}

void TFTDisplay::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t* data) {
	espi.pushImage(x,y,w,h,data);
}

void TFTDisplay::drawString(const char *s, unsigned x, unsigned y) {
	espi.setTextDatum(TL_DATUM);
	unsigned w = espi.textWidth(s);
	espi.fillRect(x, y, w, _cy, _bg);
	espi.drawString(s, x, y);
}

void TFTDisplay::flush() {
	espi.pushSprite(_xoff,_yoff);
}


