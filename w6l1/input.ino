#include <Wire.h>
#include <SPI.h>
#include <ParallaxJoystick.hpp>;

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET 4

#define CHAR_WIDTH 16
#define CHAR_HEIGHT 16

Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int DELAY_LOOP_MS = 5;
const int JOYSTICK_UPDOWN_PIN = A1;
const int JOYSTICK_LEFTRIGHT_PIN = A0;

const int MAX_ANALOG_VAL = 1023;
const enum JoystickYDirection JOYSTICK_Y_DIR = RIGHT;

ParallaxJoystick _analogJoystick(JOYSTICK_UPDOWN_PIN, JOYSTICK_LEFTRIGHT_PIN, MAX_ANALOG_VAL, JOYSTICK_Y_DIR);

const unsigned char PROGMEM chara [] = {
  0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xe8, 0x1e, 0xf8, 0x1f, 0xf8, 0x1f, 0x98, 
  0x1f, 0xf8, 0x03, 0xc0, 0x07, 0xe0, 0x07, 0xe0, 0x1b, 0xd8, 0x1a, 0x58, 0x02, 0x40, 0x02, 0x40
};

const unsigned char PROGMEM chara_walk_1 [] = {
  0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xe8, 0x1e, 0xf8, 0x1f, 0xf8, 0x1f, 0x98, 
  0x1f, 0xf8, 0x03, 0xc0, 0x07, 0xe0, 0x07, 0xe0, 0x1b, 0xd8, 0x1a, 0x58, 0x02, 0x40, 0x00, 0x40
};

const unsigned char PROGMEM chara_walk_2 [] = {
  0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xe8, 0x1e, 0xf8, 0x1f, 0xf8, 0x1f, 0x98, 
  0x1f, 0xf8, 0x03, 0xc0, 0x07, 0xe0, 0x07, 0xe0, 0x1b, 0xd8, 0x1a, 0x58, 0x02, 0x40, 0x02, 0x00
};

int x = 8;
int y = 48;
int xVel = 0;

void setup() {
  Serial.begin(9600);
  
  if (!_display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // don't proceed, loop forever
  }
  xVel = 3;
}

void loop() {
  _display.clearDisplay();

  _analogJoystick.read();
  int leftRightVal = _analogJoystick.getLeftRightVal();
  int xMovementPixels = map(leftRightVal, 0, _analogJoystick.getMaxAnalogValue() + 1, -4, 5);
  Serial.println(leftRightVal);

  if (leftRightVal > 525) {
    x += xVel;
  } else if (leftRightVal < 510) {
    x -= xVel;
  }

  _display.drawBitmap(x, y, chara, 16, 16, WHITE);
  _display.display();
}
