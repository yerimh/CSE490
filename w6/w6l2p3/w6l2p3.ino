const int VIB_OUTPUT_PIN = 6;
const int DELAY_MS = 5;
const int MAX_ANALOG_OUT = 255;

void setup() {
  // put your setup code here, to run once:
  pinMode(VIB_OUTPUT_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i <= MAX_ANALOG_OUT; i += 1) {
    analogWrite(VIB_OUTPUT_PIN, i);
    delay(DELAY_MS);

  for (int i = MAX_ANALOG_OUT; i >= 0; i -= 1) {
    analogWrite(VIB_OUTPUT_PIN, i);
    delay(DELAY_MS);
  }
  }
}
