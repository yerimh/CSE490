const int VIB_OUTPUT_PIN = 6;

void setup() {
  // put your setup code here, to run once:
  pinMode(VIB_OUTPUT_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(VIB_OUTPUT_PIN, HIGH);
  delay(1000);
  digitalWrite(VIB_OUTPUT_PIN, LOW);
  delay(1000);
}
