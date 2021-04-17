const int LED_OUTPUT_PIN = 3;
const int MAX_ANALOG_OUT = 255;
const int INTERVAL_IN_MS = 5;

int _fadeAmount = 5;
int _curBrightness = 0;
unsigned long _lastToggledTimestampMs = 0;

void setup() {
  pinMode(LED_OUTPUT_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  unsigned long currentTimestampMs = millis();
  
  if (currentTimestampMs - _lastToggledTimestampMs >= INTERVAL_IN_MS) {
    analogWrite(LED_OUTPUT_PIN, _curBrightness);
    _curBrightness = _curBrightness + _fadeAmount;
    _lastToggledTimestampMs = currentTimestampMs;

    if (_curBrightness <= 0 || _curBrightness >= MAX_ANALOG_OUT) {
      _fadeAmount = -_fadeAmount;
    }
  }

}
