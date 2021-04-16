void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentTimestampMs = millis();

  // Serial.println("Hello world!");
  
  Serial.print("Time since Arduino started: ");
  Serial.print(currentTimestampMs);
  Serial.println(" ms");
  delay(500);
}
