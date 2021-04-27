#define KEY_C 262
#define KEY_D 294
#define KEY_E 330
#define KEY_F 350
#define KEY_G 392
#define KEY_A 440
#define KEY_B 492
#define KEY_C_HIGH 523

const int OUTPUT_LED_PIN = LED_BUILTIN;
const int OUTPUT_PIEZO_PIN = 2;
const int INPUT_FSR_PIN = A0;
const int DELAY_MS = 20;

void setup() {
  // put your setup code here, to run once:
  pinMode(OUTPUT_LED_PIN, OUTPUT);
  pinMode(OUTPUT_PIEZO_PIN, OUTPUT);
  pinMode(INPUT_FSR_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int fsrVal = analogRead(INPUT_FSR_PIN);

  int ledVal = map(fsrVal, 0, 1023, 0, 255);
  int freq = map(fsrVal, 0, 1023, 50, 1500);

  if (fsrVal > 0) {
    if (freq < 293) {
      tone(OUTPUT_PIEZO_PIN, KEY_C);
    } else if (freq < 329) {
      tone(OUTPUT_PIEZO_PIN, KEY_D);
    } else if (freq < 349) {
      tone(OUTPUT_PIEZO_PIN, KEY_E);
    } else if (freq < 391) {
      tone(OUTPUT_PIEZO_PIN, KEY_F);
    } else if (freq < 439) {
      tone(OUTPUT_PIEZO_PIN, KEY_G);
    } else if (freq < 491) {
      tone(OUTPUT_PIEZO_PIN, KEY_A);
    } else if (freq < 522) {
      tone(OUTPUT_PIEZO_PIN, KEY_B);
    } else {
      tone(OUTPUT_PIEZO_PIN, KEY_C_HIGH);
    }
  } else {
    noTone(OUTPUT_PIEZO_PIN);
  }

  Serial.print(fsrVal);
  Serial.print(",");
  Serial.print(ledVal);
  Serial.print(",");
  Serial.println(freq);

  analogWrite(OUTPUT_LED_PIN, ledVal);

  delay(DELAY_MS);
}
