//importe the Gamebuino library and the gb object
#include <Gamebuino-Meta.h>

SPIClass sdSPI(VSPI);

// gamebuino setup
void setup() {
  sdSPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
	bool test = SD.begin(SD_CS, sdSPI);

  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  Serial.begin(115200);
  Serial.println("Starting gamebuino...");
  Serial.println(ESP.getCpuFreqMHz());

#if 0
	Serial.println("List dir");
	File root = SD.open("/");
	File file = root.openNextFile();
	while (file) {
		Serial.print("FILE: ");
		Serial.println(file.name());
		file = root.openNextFile();
	}

	Serial.println("opening...");
	File f = SD.open("/test.wav", "r");
	if (!f) {
		Serial.println("file open failed!");
	}
	else {
		Serial.println("file open OK!");
    uint8_t buf[20] = {0};
    f.readBytes((char*)buf, 20);
		f.close();
    for (int i = 0; i < 20; i++) {
      Serial.printf("%02x\n", buf[i]);
    }
	}
#endif

  gb.begin(test);
}

// these will be aour track identifiers. -1 means "not playing"
int8_t music = -1;
int8_t fx = -1;

/*
 Files will be opened relative to the sketch folder,
 so in this case this would be the "Audio"-folder.
 So, you will have to place the test.wav as
 "/Audio/test.wav" into the SD card, and NOT in the
 root folder!
 */


void loop() {
  while(!gb.update());

  // clear the previous screen
  gb.display.clear();
  
  // let's just print some information on free ram and CPU first
  gb.display.print("RAM:");
  gb.display.println(gb.getFreeRam());
  gb.display.print("CPU:");
  gb.display.print(gb.getCpuLoad());
  gb.display.println("%");
  
  // if we have music playing...let's print that!
  if (music != -1) {
    gb.display.println("Playing Music");
  }
  
  // here we check if the sound effects stopped playing.
  // If they did, we will set our sound effect identifier back to -1
  if (!gb.sound.isPlaying(fx)) {
    fx = -1;
  }
  // we will not be checking if the music identifier is still playing,
  // because we will start it as an infinite loop so we'll know that it
  // won't stop suddenly, as opposed to the sound effect
  
  // and now, only print that an effect is playing
  if (fx != -1) {
    gb.display.println("Playing Effect");
  }
  
  // bottom line indications
  // this just prints the lower bar for what button does what
  gb.display.setCursor(0, gb.display.height() - 5);
  gb.display.setColor(GREEN);
  gb.display.print("A");
  gb.display.setColor(BROWN);
  gb.display.print("MUSIC");
  gb.display.setColor(RED);
  gb.display.print(" B");
  gb.display.setColor(BROWN);
  gb.display.print("EFFECT");

  // okay, now we check if we want to start music
  if (gb.buttons.pressed(BUTTON_A)) {
    if (music == -1) {
      // time to start music!
      // we are starting the file "test.wav" in our sketch folder
      // gb.sound.play() will return the track identifier, which we will store into our music variable for later use
      music = gb.sound.play("/test.wav", true); // true for infinite looping
    } else {
      // music was already running, time to stop it
      gb.sound.stop(music);
      music = -1; // aaaaaand reset our track indicator
    }
  }
  
  // check if we want to start a sound effect
  if (gb.buttons.pressed(BUTTON_B)) {
    // we only want to start it if it isn't already running...
    if (fx == -1) {
      // gb.sound.playOK() also returns the track identifier
      fx = gb.sound.playOK();
    }
  }
}