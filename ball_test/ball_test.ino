#include <Wire.h>
#include <SPI.h>
#include <Shape.hpp>;
#include <ParallaxJoystick.hpp>;

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET 4

#define CHAR_WIDTH 16
#define CHAR_HEIGHT 16

#define PLATFORM_HEIGHT 4

#define SPEED 3
#define GRAVITY 1

Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int DELAY_LOOP_MS = 5;
const int JOYSTICK_UPDOWN_PIN = A1;
const int JOYSTICK_LEFTRIGHT_PIN = A0;
const int BUTTON = 5;

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
const int MAX_PLATFORM_X_SPACING_DISTANCE = 48;
const int MIN_PLATFORM_Y_SPACING_DISTANCE = -21;
const int MAX_PLATFORM_Y_SPACING_DISTANCE = 21;

int _platformSpeed = 1;
int _x = 8;
int _y = 40;
int _initVel = 12;
int yMovementPixels = 0;
int xPlatformMovement = 0;

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

enum GameState _curGameState = NEW_GAME;
enum PlayerState _curPlayerState = JUMPING;

const char STR_START[] = "Press button to start";
const char STR_DIED[] = "You died";

Rectangle _startingPlatform(0, 56, 24, PLATFORM_HEIGHT);
Ball _ball(20, 20, 5);

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON, INPUT_PULLUP);
  
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
    _display.setTextSize(1);
    _display.setTextColor(WHITE, BLACK);

    for (int i = 0; i < 3; i++) {
      _platforms[i].draw(_display);
    }
  
    int16_t x1, y1;
    uint16_t w, h;
    _display.getTextBounds(STR_START, 0, 0, &x1, &y1, &w, &h);
    _display.setCursor(_display.width() / 2 - w / 2, 15);
    _display.print(STR_START);

    if (buttonVal == LOW) {
      _curGameState = PLAYING;
    }



    
  } else if (_curGameState == PLAYING) {
    _ball.update();

    int xMaxRight = 0;
    int yMaxRight = 0;
    
    for (int i = 0; i < 3; i++) {
      _platforms[i].setX(_platforms[i].getX() - _platformSpeed);

      _platforms[i].draw(_display);

      if (xMaxRight < _platforms[i].getRight()) {
        xMaxRight = _platforms[i].getRight();
        yMaxRight = _platforms[i].getY() + 2;
      }

      if (_ball.getRight() >= _platforms[i].getLeft() && _ball.getLeft() <= _platforms[i].getRight() && _ball.getBottom() == _platforms[i].getTop()) {
        _curPlayerState = GROUNDED;
      } 
//      else if (_ball.getRight() < _platforms[i].getLeft() || _ball.getLeft() > _platforms[i].getRight()) {
//        _curPlayerState = JUMPING;
//      }
    }
    
    _analogJoystick.read();
    int leftRightVal = _analogJoystick.getLeftRightVal();
    int xMovementPixels = map(leftRightVal, 0, _analogJoystick.getMaxAnalogValue() + 1, -4, 5);
    
    _ball.setX(_ball.getX() + xMovementPixels - xPlatformMovement);
    _ball.setY(_ball.getY() + yMovementPixels);
    
    // COLLISION DETECTION
    if (_ball.getRight() >= _startingPlatform.getLeft() && _ball.getLeft() <= _startingPlatform.getRight() && _ball.getBottom() == _startingPlatform.getTop()) {
      _curPlayerState = GROUNDED;
    } 
//    else if (_ball.getRight() < _startingPlatform.getLeft() || _ball.getLeft() > _startingPlatform.getRight()) {
//      _curPlayerState = JUMPING;
//    }


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
    

    // IF PLAYER FALLS, GAME ENDS
    if (_ball.getY() > _display.height()) {
      _curGameState = GAME_OVER;
    }

    xMaxRight = max(xMaxRight, _display.width());
    for (int i = 0; i < 3; i++) {
      if (_platforms[i].getRight() < 0) {
        int platformX = xMaxRight + random(MIN_PLATFORM_X_SPACING_DISTANCE, MAX_PLATFORM_X_SPACING_DISTANCE);
        int platformWidth = random(MIN_PLATFORM_WIDTH, MAX_PLATFORM_WIDTH);
        int platformY = yMaxRight + random(MIN_PLATFORM_Y_SPACING_DISTANCE, MAX_PLATFORM_Y_SPACING_DISTANCE);      
        _platforms[i].setLocation(platformX, platformY);
        _platforms[i].setDimensions(platformWidth, PLATFORM_HEIGHT);
        _platforms[i].setDrawFill(true);
    
        xMaxRight = _platforms[i].getRight();
        yMaxRight = _platforms[i].getY();
      }
    }


    // DRAW STUFF
    _startingPlatform.draw(_display);
    _ball.draw(_display);



    
  } else if (_curGameState == GAME_OVER) {
    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setTextColor(WHITE, BLACK);
  
    int16_t x1, y1;
    uint16_t w, h;
    _display.getTextBounds(STR_START, 0, 0, &x1, &y1, &w, &h);
    _display.setCursor(_display.width() / 2 - w / 2, 15);
    _display.print(STR_DIED);

    if (buttonVal == LOW) {
      initializeGame();
      _curGameState = NEW_GAME;
      delay(300);
    }
  }
  
  _display.display();
}


// INITIALIZE GAME OBJECTS
void initializeGame() {
  _ball.setLocation(20, 20);
  _ball.setDrawBoundingBox(false);
  _ball.setDrawFill(true);

  int lastPlatformX = _startingPlatform.getRight();
  int lastPlatformY = _startingPlatform.getY();
  for (int i = 0; i < 3; i++) {
    int platformX = lastPlatformX + random(MIN_PLATFORM_X_SPACING_DISTANCE, MAX_PLATFORM_X_SPACING_DISTANCE);
    int platformY = lastPlatformY + random(MIN_PLATFORM_Y_SPACING_DISTANCE, MAX_PLATFORM_Y_SPACING_DISTANCE);
    int platformWidth = random(MIN_PLATFORM_WIDTH, MAX_PLATFORM_WIDTH);

    _platforms[i].setLocation(platformX, platformY);
    _platforms[i].setDimensions(platformWidth, PLATFORM_HEIGHT);
    _platforms[i].setDrawFill(true);

    lastPlatformX = _platforms[i].getRight();
    lastPlatformY = _platforms[i].getY() + 2;
  }

  _startingPlatform.setDrawFill(true);
}
