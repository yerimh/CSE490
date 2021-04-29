#include "src/RGBConverter/RGBConverter.h"

// BUTTON MODE
const int INPUT_BUTTON_PIN = 2;
int mode = 1;
int lastButtonState = 1;
int buttonState = 0;


// CROSSFADING RGB
const boolean COMMON_ANODE = false;
const int RGB_RED_PIN = 6;
const int RGB_GREEN_PIN = 5;
const int RGB_BLUE_PIN = 3;
const int DELAY_INTERVAL = 50;
const byte MAX_RGB_VALUE = 255;

float _hue = 0;
float _step = 0.001f;


// PHOTORESISTOR
const int PHOTOCELL_INPUT_PIN = A0;
const int MIN_PHOTOCELL_VAL = 200;
const int MAX_PHOTOCELL_VAL = 800;
const boolean PHOTOCELL_IS_R2_IN_DIVIDER = true;


// LOFI
const int LOFI_INPUT_PIN = A5;


// AMP
const int MAX_ANALOG_IN = 676;
const int MAX_ANALOG_OUT = 255;
const int AMP_INPUT_PIN = A2;
int _maxSoundLevel = 0;

RGBConverter _rgbConverter;

void setup() {
  pinMode(INPUT_BUTTON_PIN, INPUT_PULLUP);
  
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  pinMode(PHOTOCELL_INPUT_PIN, INPUT);

  pinMode(LOFI_INPUT_PIN, INPUT);

  pinMode(AMP_INPUT_PIN, INPUT);

  Serial.begin(9600);
}

void loop() {
  
  // BUTTON MODES
  int buttonState = digitalRead(INPUT_BUTTON_PIN);
  if (buttonState != lastButtonState && buttonState == 0) {
    mode++;
    if (mode > 3) {
      mode = 1;
    }
  }
  lastButtonState = buttonState;


  if (mode == 1) { // MODE 1: CROSSFADING RGB
    byte rgb[3];
    _rgbConverter.hslToRgb(_hue, 1, 0.5, rgb);
  
    int photocellVal = analogRead(PHOTOCELL_INPUT_PIN);
    int ledVal = map(photocellVal, MIN_PHOTOCELL_VAL, MAX_PHOTOCELL_VAL, 0, 255);
    ledVal = constrain(ledVal, 0, 255);
  
    if (PHOTOCELL_IS_R2_IN_DIVIDER == false) {
      ledVal = 255 - ledVal;
    }
  
    setColor(rgb[0], rgb[1], rgb[2], ledVal);
  
    _hue += _step;
  
    if (_hue > 1.0) {
      _hue = 0;
    }
  
    delay(DELAY_INTERVAL);
  } else if (mode == 2) { // MODE 2: VOLTAGE DIVIDER INPUT
    byte rgb[3];
    _rgbConverter.hslToRgb(_hue, 1, 0.5, rgb);
    
    int potVal = analogRead(LOFI_INPUT_PIN);
    float hueVal = potVal / 664.0;
    hueVal = constrain(hueVal, 0, 1);

    setColor(rgb[0], rgb[1], rgb[2], 255);
  
    _hue = hueVal;
    
    delay(DELAY_INTERVAL);
  } else { // MODE 3: MICROPHONE AMPLIFIER INPUT
    int amp = analogRead(AMP_INPUT_PIN);
    int ledVal = map(amp, MAX_ANALOG_IN/2, MAX_ANALOG_IN, 0, MAX_ANALOG_OUT);
    ledVal = constrain(ledVal, 0, MAX_ANALOG_OUT);
    setColor(255, 0, 255, ledVal);

    // crossfading rgb
//    byte rgb[3];
//    _rgbConverter.hslToRgb(_hue, 1, 0.5, rgb);
//  
//    setColor(rgb[0], rgb[1], rgb[2], ledVal);
//  
//    _hue += _step;
//  
//    if (_hue > 1.0) {
//      _hue = 0;
//    }
  }
}

// set color and brightness of LED
void setColor(int red, int green, int blue, int led) {
  float value = led / 255.0;
  if (COMMON_ANODE == true) {
    red = MAX_RGB_VALUE - red;
    green = MAX_RGB_VALUE - green;
    blue = MAX_RGB_VALUE - blue;
  }
  analogWrite(RGB_RED_PIN, red * value);
  analogWrite(RGB_GREEN_PIN, green * value);
  analogWrite(RGB_BLUE_PIN, blue * value);
}
