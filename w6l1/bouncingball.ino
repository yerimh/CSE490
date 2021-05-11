#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 _display(128, 64, &Wire, 4);

const unsigned char PROGMEM chara [] = {
  0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xe8, 0x1e, 0xf8, 0x1f, 0xf8, 0x1f, 0x98, 
  0x1f, 0xf8, 0x03, 0xc0, 0x07, 0xe0, 0x07, 0xe0, 0x1b, 0xd8, 0x1a, 0x58, 0x02, 0x40, 0x02, 0x40
};

const int _Radius = 16;
int _x = 8;
int _y = 8;
int _xSpeed = 0;
int _ySpeed = 0;

void setup() {
  _display.begin(SSD1306_SWITCHCAPVCC, 0x3D);

  _xSpeed = random(1, 4);
  _ySpeed = random(1, 4);
}

void loop() {
  _display.clearDisplay();

  _x += _xSpeed;
  _y += _ySpeed;

  if (_x <= 0 || _x + _Radius >= _display.width()) {
    _xSpeed = _xSpeed * -1;
  }

  if (_y <= 0 || _y + _Radius >= _display.height()) {
    _ySpeed = _ySpeed * -1;
  }

  _display.drawBitmap(_x, _y, chara, 16, 16, WHITE);
  
  _display.display();
}
