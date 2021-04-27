const int LED_OUTPUT_PIN = 3;
const int VAR_INPUT_PIN = A0;

void setup()
{
  // pinMode(VAR_INPUT_PIN, INPUT);
  pinMode(LED_OUTPUT_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  int potVal = analogRead(A0);
  Serial.println(potVal);
  analogWrite(LED_OUTPUT_PIN, potVal);
  delay(50);
}
