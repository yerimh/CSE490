const int LED_OUTPUT_PIN = 3;
void setup() {
  // put your setup code here, to run once:

  pinMode(LED_OUTPUT_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(3, HIGH); // turn LED on (output 5V)
  Serial.println("HIGH");
  delay(1000);
  digitalWrite(3, LOW); // turn LED off (output 0V)
  Serial.println("LOW");
  delay(1000);
}
