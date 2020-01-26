#include <Arduino.h>
#include <Gamebuino-Meta.h>

int16_t width = 0;
int16_t height = 0;

void setup() {
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);
    Serial.begin(115200);
    Serial.println("Starting gamebuino...");
    gb.begin();
    Serial.println("Object init'd");
    gb.setScreenRotation(Rotation::right);
    width = gb.display.width();
    height = gb.display.height();
}

void loop() {
    gb.waitForUpdate();
    gb.display.clear();
    
    for (int16_t i = 0; i < width; i++) {
        for (int16_t j = 0; j < height; j++) {
            if (j % 2 == 0) {
                gb.display.setColor(ColorIndex::red);
            }
            else {
                gb.display.setColor(ColorIndex::yellow);
            }

            gb.display.drawPixel(i,j);
        }
    }
}