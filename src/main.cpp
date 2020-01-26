#include <Gamebuino-ESP32.h>

Gamebuino_ESP32 gb;

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