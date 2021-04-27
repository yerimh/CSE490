const int PULLDOWN_BUTTON_PIN = 2;
const int PULLDOWN_OUTPUT_LED_PIN = 3;

const int PULLUP_BUTTON_PIN = 4;
const int PULLUP_OUTPUT_LED_PIN = 5;

const int INT_BUTTON_PIN = 8;
const int INT_OUTPUT_LED_PIN = 9;

void setup() {
  pinMode(PULLDOWN_BUTTON_PIN, INPUT);
  pinMode(PULLDOWN_OUTPUT_LED_PIN, OUTPUT);

  pinMode(PULLUP_BUTTON_PIN, INPUT);
  pinMode(PULLUP_OUTPUT_LED_PIN, OUTPUT);

  pinMode(INT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(INT_OUTPUT_LED_PIN, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  int pulldownButtonState = digitalRead(PULLDOWN_BUTTON_PIN);
  digitalWrite(PULLDOWN_OUTPUT_LED_PIN, pulldownButtonState);

  int pullupButtonState = !(digitalRead(PULLUP_BUTTON_PIN));
  digitalWrite(PULLUP_OUTPUT_LED_PIN, pullupButtonState);

  int internalButtonState = !(digitalRead(INT_BUTTON_PIN));
  digitalWrite(INT_OUTPUT_LED_PIN, internalButtonState);
  
  delay(30);
}
