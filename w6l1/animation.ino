#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET 4

Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const unsigned char PROGMEM chara_walk_1 [] = {
  0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xe8, 0x1e, 0xf8, 0x1f, 0xf8, 0x1f, 0x98, 
  0x1f, 0xf8, 0x03, 0xc0, 0x07, 0xe0, 0x07, 0xe0, 0x1b, 0xd8, 0x1a, 0x58, 0x02, 0x40, 0x00, 0x40
};

const unsigned char PROGMEM chara_walk_2 [] = {
  0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xe8, 0x1e, 0xf8, 0x1f, 0xf8, 0x1f, 0x98, 
  0x1f, 0xf8, 0x03, 0xc0, 0x07, 0xe0, 0x07, 0xe0, 0x1b, 0xd8, 0x1a, 0x58, 0x02, 0x40, 0x02, 0x00
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // initialize display
  if (!_display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // don't proceed, loop forever
  }
}

void loop() {
  // clear the display
  _display.clearDisplay();
  _display.drawBitmap(0, 0, chara_walk_1, 16, 16, WHITE);
  _display.display();
  delay(200);
  _display.clearDisplay();
  _display.drawBitmap(0, 0, chara_walk_2, 16, 16, WHITE);
  _display.display();
  delay(200);
}
