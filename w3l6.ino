const boolean IS_COMMON_ANODE = false;

const int DELAY_MS = 1000;
const int RGB_RED_LED_PIN = 6;
const int RGB_GREEN_LED_PIN = 5;
const int RGB_BLUE_LED_PIN = 3;

void setup() {
  pinMode(RGB_RED_LED_PIN, OUTPUT);
  pinMode(RGB_BLUE_LED_PIN, OUTPUT);
  pinMode(RGB_GREEN_LED_PIN, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // red
  Serial.print("Red: ");
  setRgbLedColor(HIGH, LOW, LOW);
  delay(DELAY_MS);

  // green
  Serial.print("Green: ");
  setRgbLedColor(LOW, HIGH, LOW);
  delay(DELAY_MS);

  // blue
  Serial.print("Blue: ");
  setRgbLedColor(LOW, LOW, HIGH);
  delay(DELAY_MS);

  // purple
  Serial.print("Purple: ");
  setRgbLedColor(HIGH, LOW, HIGH);
  delay(DELAY_MS);

  // turqoise
  Serial.print("Turqoise: ");
  setRgbLedColor(LOW, HIGH, HIGH);
  delay(DELAY_MS);

  // yellow
  Serial.print("Yellow: ");
  setRgbLedColor(HIGH, HIGH, LOW);
  delay(DELAY_MS);

  // white
  Serial.print("White: ");
  setRgbLedColor(HIGH, HIGH, HIGH);
  delay(DELAY_MS);
}

// Function expects either HIGH or LOW for each parameter
void setRgbLedColor(int red, int green, int blue){
  if (IS_COMMON_ANODE == true) {
    red = !red;
    green = !green;
    blue = !blue;
  }

  Serial.print(red);
  Serial.print(", ");
  Serial.print(green);
  Serial.print(", ");
  Serial.println(blue);
  
  digitalWrite(RGB_RED_LED_PIN, red);
  digitalWrite(RGB_GREEN_LED_PIN, green);
  digitalWrite(RGB_BLUE_LED_PIN, blue);
}
