#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int DELAY_MS = 5;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET 4
Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int OUTPUT_PIN = LED_BUILTIN;

void setup() {
  Serial.begin(9600);
  pinMode(OUTPUT_PIN, OUTPUT);

  if (!_display.begin(SSD1306_SWITCHCAPVCC, 0X3D)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setTextColor(SSD1306_WHITE);
  _display.setCursor(0, 0);
  _display.print("Waiting to receive\ndata from serial...");
  _display.display();
}

void loop() {
  if (Serial.available() > 0) {
    String rcvdSerialData = Serial.readStringUntil('\n');

    int ledValue = rcvdSerialData.toInt();
    ledValue = constrain(ledValue, 0, 255);
    analogWrite(OUTPUT_PIN, ledValue);

    _display.clearDisplay();
    int16_t x1, y1;
    uint16_t textWidth, textHeight;

    _display.setTextSize(2);

    _display.getTextBounds("Test", 0, 0, &x1, &y1, &textWidth, &textHeight);
    int oneLineOfTextHeight = textHeight;

    _display.getTextBounds(rcvdSerialData, 0, 0, &x1, &y1, &textWidth, &textHeight);

    if (textHeight <= oneLineOfTextHeight) {
      uint16_t yText = _display.height() / 2 - textHeight / 2;
      uint16_t xText = _display.width() / 2 - textWidth / 2;
      _display.setCursor(xText, yText);
      _display.print(rcvdSerialData);
      _display.display();
    } else {
      _display.setTextSize(1);
      _display.setCursor(0, 0);
      _display.print(rcvdSerialData);
      _display.display();
    }

    uint16_t yText = _display.height() / 2 - textHeight / 2;
    uint16_t xText = _display.width() / 2 - textWidth / 2;

    Serial.print("Arduino received: '");
    Serial.print(rcvdSerialData);
    Serial.println("'");
  }

  delay(DELAY_MS);
}
