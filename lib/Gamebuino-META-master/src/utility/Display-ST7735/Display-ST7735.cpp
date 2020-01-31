/***************************************************
	This is a library for the Adafruit 1.8" SPI display.

This library works with the Adafruit 1.8" TFT Breakout w/SD card
	----> http://www.adafruit.com/products/358
The 1.8" TFT shield
	----> https://www.adafruit.com/product/802
The 1.44" TFT breakout
	----> https://www.adafruit.com/product/2088
as well as Adafruit raw 1.8" TFT display
	----> http://www.adafruit.com/products/618

	Check out the links above for our tutorials and wiring diagrams
	These displays use SPI to communicate, 4 or 5 pins are required to
	interface (RST is optional)
	Adafruit invests time and resources providing this open source code,
	please support Adafruit and open-source hardware by purchasing
	products from Adafruit!

	Written by Limor Fried/Ladyada for Adafruit Industries.
	MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "Display-ST7735.h"
#include <limits.h>
#include "../Image.h"

namespace Gamebuino_Meta {

inline uint16_t swapcolor(uint16_t x) { 
	return (x << 11) | (x & 0x07E0) | (x >> 11);
}

// Constructor when using hardware SPI.	Faster, but must use SPI pins
// specific to each board type (e.g. 11,13 for Uno, 51,52 for Mega, etc.)
// Display_ST7735::Display_ST7735(int8_t cs, int8_t rs)  : Graphics(ST7735_TFTHEIGHT_18, ST7735_TFTWIDTH) {
// Display_ST7735::Display_ST7735(int8_t cs, int8_t rs)  : Graphics(80*3, 64*3) {
Display_ST7735::Display_ST7735(int8_t cs, int8_t rs)  : Graphics(80, 64) {
	// we use the pinmask vars as we don't need cs / rs after we have the pinmask
}

// Initialization for ST7735R screens (green or red tabs)
void Display_ST7735::init() {
	_tft.init();
	_tft.fillScreen(0);
}

void Display_ST7735::setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	_tft.setAddrWindow(x0, y0, x1-x0, y1-y0);
}

void Display_ST7735::pushColor(uint16_t c) {
	_tft.pushColor(c);
}

void Display_ST7735::_drawPixel(int16_t x, int16_t y) {
	_tft.drawPixel(x,y,(uint16_t)color.c);
}

void Display_ST7735::drawFastVLine(int16_t x, int16_t y, int16_t h) {
	_tft.drawFastVLine(x,y,h,(uint32_t)color.c);
}

void Display_ST7735::drawFastHLine(int16_t x, int16_t y, int16_t w) {
	_tft.drawFastHLine(x,y,w,(uint32_t)color.c);
}

void Display_ST7735::drawBufferedLine(int16_t x, int16_t y, uint16_t *buffer, uint16_t w, Image& img) {
	//create a local buffer line not to mess up the source
	uint16_t bufferedLine[w];
	for (uint16_t i = 0; i < w; i++) {
		uint16_t color = buffer[i];
		color = (color << 8) | (color >> 8); //change endianness
		bufferedLine[i] = color;
	}

	setAddrWindow(x, y, x + w - 1, y + 1);
	sendBuffer(bufferedLine, w);
}

void Display_ST7735::drawBuffer(int16_t x, int16_t y, uint16_t *buffer, uint16_t w, uint16_t h) {
	setAddrWindow(x, y, x + w - 1, y + h - 1);
	sendBuffer(buffer, w*h);
}

void Display_ST7735::sendBuffer(uint16_t *buffer, uint16_t n) {
	_tft.pushColors(buffer, n, false);	
}

void Display_ST7735::drawImage(int16_t x, int16_t y, Image& img) {
	img.nextFrame();
	int16_t w = img._width;
	int16_t h = img._height;
	if (w == 0 || h == 0) {
		return;
	}
	
	Graphics::drawImage(x, y, img); //fallback to the usual
}

void Display_ST7735::drawImage(int16_t x, int16_t y, Image& img, int16_t w2, int16_t h2) {
	// Serial.printf("x: %d y: %d img.w: %d img.h: %d w2: %d h2: %d\n", x, y, img._width, img._height, w2, h2);
	img.nextFrame();
	//out of screen
	if ((x > _width) || ((x + abs(w2)) < 0) || (y > _height) || ((y + abs(h2)) < 0) || (w2 == 0) || (h2 == 0)) return;

	int16_t w = img._width;
	int16_t h = img._height;
	if (w == 0 || h == 0) {
		return;
	}

	//no scaling
	if ((w == w2) && (h == h2)) { 
		drawImage(x, y, img);
		return;
	}

	//x2 upscaling to full screen
	if ((w2 == (w * 2)) && (h2 == (h * 2)) && (_width == w2) && (_height == h2)) {

		//set the window to the whole screen
		setAddrWindow(0, 0, _width - 1, _height - 1);
		
		if (img.colorMode == ColorMode::rgb565) {
			_tft.pushColors(img._buffer, w*h, false);
		}
		if (img.colorMode == ColorMode::index) {
			_tft.pushColors(img._buffer, w*h, false);
		}
	}

	// fall back to most generic but slow resizing
	Graphics::drawImage(x, y, img, w2, h2);
}

void Display_ST7735::fillRect(int16_t x, int16_t y, int16_t w, int16_t h) {
	_tft.fillRect(x,y,w,h,(uint32_t)color.c);
}

void Display_ST7735::setRotation(Rotation r) {
	_tft.setRotation((uint8_t)r);
	// _tft.setRotation(3);
}

void Display_ST7735::invertDisplay(bool i) {
	_tft.invertDisplay(i);
}

Rotation Display_ST7735::getRotation() {
	return (Rotation)_tft.getRotation();
}

} // namespace Gamebuino_Meta
