#include <Arduino.h>
#include <Gamebuino-Meta.h>

void setup() {
    Serial.begin(115200);
    Serial.println("Starting gamebuino...");

    gb.begin();
}

void loop() {
    gb.waitForUpdate();
    gb.display.clear();
    gb.display.print("Hello World!");
}