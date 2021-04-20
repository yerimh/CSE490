#include "src/RGBConverter/RGBConverter.h"

const boolean COMMON_ANODE = false;
const int RGB_RED_PIN = 6;
const int RGB_GREEN_PIN = 5;
const int RGB_BLUE_PIN = 3;
const int DELAY_INTERVAL = 50;
const byte MAX_RGB_VALUE = 255;

float _hue = 0;
float _step = 0.001f;

RGBConverter _rgbConverter;

void setup() {
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  byte rgb[3];
  _rgbConverter.hslToRgb(_hue, 1, 0.5, rgb);

  Serial.print("hue=");
  Serial.print(_hue);
  Serial.print(" r=");
  Serial.print(rgb[0]);
  Serial.print(" g=");
  Serial.print(rgb[1]);
  Serial.print(" b=");
  Serial.println(rgb[2]);

  setColor(rgb[0], rgb[1], rgb[2]);

  _hue += _step;

  if (_hue > 1.0) {
    _hue = 0;
  }

  delay(DELAY_INTERVAL);
}

void setColor(int red, int green, int blue) {
  if (COMMON_ANODE == true) {
    red = MAX_RGB_VALUE - red;
    green = MAX_RGB_VALUE - green;
    blue = MAX_RGB_VALUE - blue;
  }
  analogWrite(RGB_RED_PIN, red);
  analogWrite(RGB_GREEN_PIN, green);
  analogWrite(RGB_BLUE_PIN, blue);
}
