const int VIB_OUTPUT_PIN = 6;
const int VAR_INPUT_PIN = A2;

void setup() {
  // put your setup code here, to run once:
  pinMode(VIB_OUTPUT_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int potVal = analogRead(A2);
  analogWrite(VIB_OUTPUT_PIN, potVal);
  delay(50);
}
