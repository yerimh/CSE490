// Credit to a huge chunk of this code goes to
// Jon Froehlich's code from Flappy Bird

#include <Wire.h>
#include <SPI.h>
#include <Shape.hpp>
#include <ParallaxJoystick.hpp>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET 4

#define CHAR_WIDTH 10
#define CHAR_HEIGHT 16

#define PLATFORM_HEIGHT 4

#define SPEED 3
#define GRAVITY 2

#define KEY_G 392
#define KEY_F 350
#define KEY_E 330

Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int DELAY_LOOP_MS = 5;
const int JOYSTICK_UPDOWN_PIN = A1;
const int JOYSTICK_LEFTRIGHT_PIN = A0;
const int BUTTON = 5;
const int VIB_PIN = 6;
const int PIEZO_PIN = 9;
const int LED_PIN = 10;

const int MAX_ANALOG_VAL = 1023;
const enum JoystickYDirection JOYSTICK_Y_DIR = RIGHT;

ParallaxJoystick _analogJoystick(JOYSTICK_UPDOWN_PIN, JOYSTICK_LEFTRIGHT_PIN, MAX_ANALOG_VAL, JOYSTICK_Y_DIR);

// fps
float _fps = 0;
unsigned long _frameCount = 0;
unsigned long _fpsStartTimeStamp = 0;
const boolean _drawFps = false;

const int MIN_PLATFORM_WIDTH = 8;
const int MAX_PLATFORM_WIDTH = 36;
const int MIN_PLATFORM_X_SPACING_DISTANCE = 24;
const int MAX_PLATFORM_X_SPACING_DISTANCE = 32;
const int MIN_PLATFORM_Y_SPACING_DISTANCE = -21;
const int MAX_PLATFORM_Y_SPACING_DISTANCE = 8;

int _platformSpeed = 1;
int _x = 8;
int _y = 40;
int _initVel = 12;
int yMovementPixels = 0;
int xPlatformMovement = 0;
int score = 0;
int death_count = 0;
int ledState = LOW;

Rectangle _platforms[3] = { Rectangle(0, 0, 0, 0),
                            Rectangle(0, 0, 0, 0),
                            Rectangle(0, 0, 0, 0)
                          };

enum GameState {
  NEW_GAME,
  PLAYING,
  GAME_OVER
};

enum PlayerState {
  GROUNDED,
  JUMPING
};

enum PlayerDirection {
  RIGHT_IDLE,
  RIGHT_RUN,
  LEFT_IDLE,
  LEFT_RUN
};

enum GameState _curGameState = NEW_GAME;
enum GameState _prevGameState = _curGameState;
enum PlayerState _curPlayerState = JUMPING;
enum PlayerState _prevPlayerState = _curPlayerState;
enum PlayerDirection _curPlayerDirection = RIGHT_RUN;


const char STR_START[] = "Press button to start";


const char STR_DIED1[] = "You died";
const char STR_DIED3[] = "Bruh";
const char STR_DIED4[] = "sheeeeeeeesh";
const char STR_DIED5[] = "you know when you dieyou're supposed to   like stay dead";
const char STR_DIED6[] = "you're persistent,   aren'tcha?";
const char STR_DIED7[] = "i'll be honest i'm   getting kinda bored  tho";
const char STR_DIED8[] = "oop sorry just wantedto try something. ok let me fix it";
const char STR_DIED9[] = "ah shit uhhh ok i    don't know how to fixit sorry";
const char STR_DIED10[] = "eh just click reset  or something";


const char STR_TRAVELED[] = "You traveled ";
const char STR_PIXELS[] = " pixels";

unsigned char _player_right[] = {
  0xff, 0xc0, 0xff, 0xc0, 0xff, 0xc0, 0xff, 0xc0, 0xff, 0x40, 0xf7, 0xc0, 0xff, 0xc0, 0xfc, 0xc0, 
  0xff, 0xc0, 0x1e, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0xde, 0xc0, 0xd2, 0xc0, 0x12, 0x00, 0x12, 0x00
};

unsigned char _player_left[] = {
  0xff, 0xc0, 0xff, 0xc0, 0xff, 0xc0, 0xff, 0xc0, 0xbf, 0xc0, 0xfb, 0xc0, 0xff, 0xc0, 0xcf, 0xc0, 
  0xff, 0xc0, 0x1e, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0xde, 0xc0, 0xd2, 0xc0, 0x12, 0x00, 0x12, 0x00
};

const unsigned char PROGMEM _player_glitch[] = {
  0xad, 0xc0, 0xf6, 0x80, 0x27, 0x80, 0xbd, 0xc0, 0x5c, 0x00, 0x34, 0x40, 0x17, 0x40, 0x44, 0x00, 
  0x77, 0x40, 0x13, 0x00, 0x14, 0x00, 0x3a, 0x80, 0xc9, 0xc0, 0x09, 0x00, 0x12, 0x00, 0x22, 0xc0
};

class Sprite : public Shape {

  protected:

    int _numFrames = 2;
    int _curFrame = 0;
    unsigned char *_frame1;
    unsigned char *_frame2;
    const unsigned char *_frame3;

  public:

    Sprite(unsigned char *frame1, unsigned char *frame2, const unsigned char *frame3, int x, int y, int width, int height)
      : Shape(x, y, width, height)
    {
      _frame1 = frame1;
      _frame2 = frame2;
      _frame3 = frame3;
    }

    void draw(Adafruit_SSD1306& display) {
      if (death_count < 7) {
        if (_curPlayerDirection == RIGHT_IDLE) {
          _player_right[30] = 0x12;
          _display.drawBitmap(_x, _y, _frame1, 10, 16, WHITE);
        } else if (_curPlayerDirection == RIGHT_RUN) {
          _display.drawBitmap(_x, _y, _frame1, 10, 16, WHITE);
          if (_curFrame == 0) {
            _player_right[30] = 0x10;
          } else {
            _player_right[30] = 0x02;
          }
        } else if (_curPlayerDirection == LEFT_IDLE) {
          _player_right[30] = 0x12;
          _display.drawBitmap(_x, _y, _frame2, 10, 16, WHITE);
        } else if (_curPlayerDirection == LEFT_RUN) {
          _display.drawBitmap(_x, _y, _frame2, 10, 16, WHITE);
          if (_curFrame == 0) {
            _player_left[30] = 0x02;
          } else {
            _player_left[30] = 0x10;
          }
        }
  
        _curFrame++;
  
        if (_curFrame >= _numFrames) {
          _curFrame = 0;
        }
      } else {
        _display.drawBitmap(_x, _y, _frame3, 10, 16, WHITE);
      }
      
    }
};

Sprite _player(_player_right, _player_left, _player_glitch, 0, 0, 10, 16);


void setup() {
  Serial.begin(9600);

  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(VIB_PIN, OUTPUT);
  pinMode(PIEZO_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  if (!_display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  initializeGame();

  _fpsStartTimeStamp = millis();
}

void loop() {
  
  _display.clearDisplay();

  int16_t x1, y1;
  uint16_t w, h;

  int buttonVal = digitalRead(BUTTON);

  if (_curGameState == NEW_GAME) {
    _display.clearDisplay();
    if (death_count < 10) {
      score = 0;
      _display.setTextSize(1);
      _display.setTextColor(WHITE, BLACK);
    
      int16_t x1, y1;
      uint16_t w, h;
      _display.getTextBounds(STR_START, 0, 0, &x1, &y1, &w, &h);
      _display.setCursor(_display.width() / 2 - w / 2, 10);
      _display.print(STR_START);

      for (int i = 0; i < 3; i++) {
        _platforms[i].draw(_display);
      }
  
      if (buttonVal == LOW) {
        _curGameState = PLAYING;
        delay(300);
      }

      _player.draw(_display);
    }


  } else if (_curGameState == PLAYING) {

    // PLATFORMS
    int xMaxRight = 0;
    int yMaxRight = 0;
    
    for (int i = 0; i < 3; i++) {
      _platforms[i].draw(_display);     
      _platforms[i].setX(_platforms[i].getX() - _platformSpeed);

      if (xMaxRight < _platforms[i].getRight()) {
        xMaxRight = _platforms[i].getRight();
        yMaxRight = _platforms[i].getY() + 2;
      }
    }

    score++;



    // LED BLINK
    unsigned long previousTimeLed;
    unsigned long currentTimeLed;
    if (score % 500 == 0) {
      currentTimeLed = millis();
      ledState = HIGH;
    }

    previousTimeLed = millis();

    if (currentTimeLed - previousTimeLed >= 1000) {
      previousTimeLed = currentTimeLed;

      ledState = LOW;
    }

    digitalWrite(LED_PIN, ledState);



    // JOYSTICK CONTROLS    
    _analogJoystick.read();
    int leftRightVal = _analogJoystick.getLeftRightVal();
    int xMovementPixels = map(leftRightVal, 0, _analogJoystick.getMaxAnalogValue() + 1, -4, 5);
    if (xMovementPixels < 0) {
      _curPlayerDirection = LEFT_RUN;
    } else if (xMovementPixels > 0) {
      _curPlayerDirection = RIGHT_RUN;
    } else {
      if (_curPlayerDirection == RIGHT_RUN && _curPlayerState == GROUNDED) {
        _curPlayerDirection = RIGHT_IDLE;
      } else if (_curPlayerDirection == LEFT_RUN && _curPlayerState == GROUNDED) {
        _curPlayerDirection = LEFT_IDLE;
      }
    }

    _player.setX(_player.getX() + xMovementPixels - xPlatformMovement);
    _player.setY(_player.getY() + yMovementPixels);



    // COLLISION
    for (int i = 0; i < 3; i++) {
      if (_player.getRight() >= _platforms[i].getLeft() && _player.getLeft() <= _platforms[i].getRight() && _player.getBottom() == _platforms[i].getTop()) {
        _curPlayerState = GROUNDED;
        break;
        } 
      else {
        _curPlayerState = JUMPING;
      }
    }

    unsigned long vibTime;
    if (_prevPlayerState == JUMPING && _curPlayerState == GROUNDED) {
      vibTime = millis();
      digitalWrite(VIB_PIN, 5);
    }
    unsigned long curTime = millis();
    if (curTime - vibTime >= 100) {
      digitalWrite(VIB_PIN, 0);
    }

    _prevPlayerState = _curPlayerState;


    // ON THE GROUND
    if (_curPlayerState == GROUNDED) {
      xPlatformMovement = _platformSpeed;
      yMovementPixels = 0;
      if (buttonVal == LOW) {
        _initVel = 8;
        yMovementPixels = -_initVel;
        _curPlayerState = JUMPING;
      }
    }
    // IN THE AIR
    if (_curPlayerState == JUMPING) {
      xPlatformMovement = 0;
      if (yMovementPixels < 0) {
        yMovementPixels = -_initVel;
        _initVel = _initVel / 2;
      } else {
        yMovementPixels = 1;
      }
    }
    

    // DYING CONDITION
    if (_player.getY() > _display.height() || _player.getRight() < 0 || _player.getLeft() > _display.width()) {
      _curGameState = GAME_OVER;
      death_count++;
    }


    // PLATFORM GENERATION
    xMaxRight = max(xMaxRight, _display.width());
    for (int i = 0; i < 3; i++) {
      if (_platforms[i].getRight() < 0) {
        int platformX = xMaxRight + random(MIN_PLATFORM_X_SPACING_DISTANCE, MAX_PLATFORM_X_SPACING_DISTANCE);
        int platformWidth = random(MIN_PLATFORM_WIDTH, MAX_PLATFORM_WIDTH);
        int platformY = yMaxRight + random(MIN_PLATFORM_Y_SPACING_DISTANCE, MAX_PLATFORM_Y_SPACING_DISTANCE);      
        _platforms[i].setLocation(platformX, platformY);
        // clamp
        if (_platforms[i].getY() > SCREEN_HEIGHT - PLATFORM_HEIGHT) {
          _platforms[i].setY(SCREEN_HEIGHT - PLATFORM_HEIGHT);
        }
        if (_platforms[i].getY() < 36) {
          _platforms[i].setY(36);
        }
        _platforms[i].setDimensions(platformWidth, PLATFORM_HEIGHT);
        _platforms[i].setDrawFill(true);
    
        xMaxRight = _platforms[i].getRight();
        yMaxRight = _platforms[i].getY() + 2;
      }
    }


    // DRAW STUFF
    _player.draw(_display);



    // GAME OVER
  } else if (_curGameState == GAME_OVER) {
    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setTextColor(WHITE, BLACK);
  
    int16_t x1, y1;
    uint16_t w, h;
    int yText = 15;
    _display.getTextBounds(STR_DIED1, 0, 0, &x1, &y1, &w, &h);
    if (death_count < 3) {
      _display.setCursor(_display.width() / 2 - w / 2, yText);
    } else {
      _display.setCursor(0, yText);
    }
    switch (death_count) {
      case 1:
        _display.print(STR_DIED1);
        break;
      case 2:
        _display.print(STR_DIED1);
        break;
      case 3:
        _display.print(STR_DIED3);
        break;
      case 4:
        _display.print(STR_DIED4);
        break;
      case 5:
        _display.print(STR_DIED5);
        break;
      case 6:
        _display.print(STR_DIED6);
        break;
      case 7:
        _display.print(STR_DIED7);
        break;
      case 8:
        _display.print(STR_DIED8);
        break;
      case 9:
        _display.print(STR_DIED9);
        break;
      case 10:
        _display.print(STR_DIED10);
        break;
    }

    if (death_count < 3) {
      yText = yText + h + 1;
      _display.getTextBounds(STR_TRAVELED, 0, 0, &x1, &y1, &w, &h);
      _display.setCursor(_display.width() / 2 - w / 2, yText);
      _display.print(STR_TRAVELED);
      _display.print(score);

      yText = yText + h + 1;
      _display.getTextBounds(STR_PIXELS, 0, 0, &x1, &y1, &w, &h);
      _display.setCursor(_display.width() / 2 - w / 2, yText);
      _display.print(STR_PIXELS);
    }

    if (buttonVal == LOW) {
      initializeGame();
      _curGameState = NEW_GAME;
      delay(300);
    }
  }
  
  _display.display();


  // PLAY TONE
  if (_prevGameState == PLAYING && _curGameState == GAME_OVER) {
    tone(PIEZO_PIN, KEY_G);
    delay(200);
    tone(PIEZO_PIN, KEY_F);
    delay(200);
    tone(PIEZO_PIN, KEY_E, 500);
  }
  _prevGameState = _curGameState;
}


// INITIALIZE GAME OBJECTS
void initializeGame() {  
  int lastPlatformX = 24;
  int lastPlatformY = 36;
  for (int i = 0; i < 3; i++) {
    int platformX = lastPlatformX + random(MIN_PLATFORM_X_SPACING_DISTANCE, MAX_PLATFORM_X_SPACING_DISTANCE);
    int platformY = lastPlatformY + random(MIN_PLATFORM_Y_SPACING_DISTANCE, MAX_PLATFORM_Y_SPACING_DISTANCE);
    int platformWidth = random(MIN_PLATFORM_WIDTH, MAX_PLATFORM_WIDTH);

    _platforms[i].setLocation(platformX, platformY);
    if (i == 0) {
      _player.setLocation(platformX, platformY - CHAR_HEIGHT - 2);
    }
    // clamp
    if (_platforms[i].getY() > SCREEN_HEIGHT - PLATFORM_HEIGHT) {
      _platforms[i].setY(SCREEN_HEIGHT - PLATFORM_HEIGHT);
    }
    if (_platforms[i].getY() < 36) {
      _platforms[i].setY(36);
    }
    _platforms[i].setDimensions(platformWidth, PLATFORM_HEIGHT);
    _platforms[i].setDrawFill(true);

    lastPlatformX = _platforms[i].getRight();
    lastPlatformY = _platforms[i].getY() + 2;
  }
}
