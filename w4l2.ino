#define KEY_C 262
#define KEY_D 294
#define KEY_E 330
#define KEY_F 350
#define KEY_G 392
#define KEY_A 440
#define KEY_B 492
#define KEY_C_HIGH 523

const int INPUT_BUTTON_C_PIN = 2;
const int INPUT_BUTTON_D_PIN = 3;
const int INPUT_BUTTON_E_PIN = 4;
const int INPUT_BUTTON_F_PIN = 5;
const int INPUT_BUTTON_G_PIN = 6;
const int INPUT_BUTTON_A_PIN = 8;
const int INPUT_BUTTON_B_PIN = 9;
const int INPUT_BUTTON_C_HIGH_PIN = 10;

const int DEBOUNCE_WINDOW = 40; // in ms

const int OUTPUT_PIEZO_PIN = 11;
const int OUTPUT_LED_PIN = LED_BUILTIN;

const boolean _buttonsAreActiveLow = true;

void setup() {
  pinMode(INPUT_BUTTON_C_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_D_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_E_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_F_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_G_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_B_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_C_HIGH_PIN, INPUT_PULLUP);
  pinMode(OUTPUT_PIEZO_PIN, OUTPUT);
  pinMode(OUTPUT_LED_PIN, OUTPUT);
}

void loop() {
  if (isButtonPressed(INPUT_BUTTON_C_PIN)) {
    tone(OUTPUT_PIEZO_PIN, KEY_C);
  } else if (isButtonPressed(INPUT_BUTTON_D_PIN)) {
    tone(OUTPUT_PIEZO_PIN, KEY_D);
  } else if (isButtonPressed(INPUT_BUTTON_E_PIN)) {
    tone(OUTPUT_PIEZO_PIN, KEY_E);
  } else if (isButtonPressed(INPUT_BUTTON_F_PIN)) {
    tone(OUTPUT_PIEZO_PIN, KEY_F);
  } else if (isButtonPressed(INPUT_BUTTON_G_PIN)) {
    tone(OUTPUT_PIEZO_PIN, KEY_G);
  } else if (isButtonPressed(INPUT_BUTTON_A_PIN)) {
    tone(OUTPUT_PIEZO_PIN, KEY_A);
  } else if (isButtonPressed(INPUT_BUTTON_B_PIN)) {
    tone(OUTPUT_PIEZO_PIN, KEY_B);
  } else if (isButtonPressed(INPUT_BUTTON_C_HIGH_PIN)) {
    tone(OUTPUT_PIEZO_PIN, KEY_C_HIGH);
  } else {
    noTone(OUTPUT_PIEZO_PIN);
    digitalWrite(OUTPUT_LED_PIN, LOW);
  }
}

boolean isButtonPressed(int btnPin) {
  int btnVal = digitalRead(btnPin);
  delay(DEBOUNCE_WINDOW);
  int btnVal2 = digitalRead(btnPin);
  if (btnVal == btnVal2) {
    if (_buttonsAreActiveLow && btnVal == LOW) {
      digitalWrite(OUTPUT_LED_PIN, HIGH);
      return true;
    } else if (!_buttonsAreActiveLow && btnVal == HIGH) {
      digitalWrite(OUTPUT_LED_PIN, HIGH);
      return true;
    }
  }

  return false;
}
