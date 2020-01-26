#pragma once

#include <Arduino.h>
#include "Image.h"

class Gamebuino_ESP32 {

public:
	// Buttons buttons;
	Image display = DISPLAY_CONSTRUCTOR;
	// Sound sound;
	Display_ST7735 tft = Display_ST7735(TFT_CS, TFT_DC);
	Image lights = Image(2, 4, ColorMode::rgb565);
	// Save save;
	// Save settings;
	// Language language;
	// Bootloader bootloader;
	// Gui gui;
	// MetaMode metaMode;
	// Collide collide;

	void begin();
	void startScreen();
	void titleScreen();
	bool update();
	void waitForUpdate();
	void updateDisplay();
	uint8_t startMenuTimer;
	uint32_t frameCount;
	void setFrameRate(uint8_t fps);
	void pickRandomSeed();
	
	uint8_t getCpuLoad();
	uint16_t getFreeRam();
	uint32_t frameDurationMicros;
	uint32_t frameStartMicros;
	bool frameEndFlag;
	
	void checkHomeMenu();
	void homeMenu();
	
	void changeGame();
	void getDefaultName(char* string);
	
	bool collidePointRect(int16_t x1, int16_t y1 ,int16_t x2 ,int16_t y2, int16_t w, int16_t h);  // deprecated. Use collide.pointRect() instead
	bool collideRectRect(int16_t x1, int16_t y1, int16_t w1, int16_t h1 ,int16_t x2 ,int16_t y2, int16_t w2, int16_t h2);  // deprecated. Use collide.rectRect() instead
	bool collideBitmapBitmap(int16_t x1, int16_t y1, const uint8_t* b1, int16_t x2, int16_t y2, const uint8_t* b2);
	
	// Color createColor(uint8_t r, uint8_t g, uint8_t b);
	// uint8_t getTimePerFrame();
	
	// void setScreenRotation(Rotation r);
	// Rotation getScreenRotation();
	
	bool inited = false;
	bool sdInited = false;

private:

};