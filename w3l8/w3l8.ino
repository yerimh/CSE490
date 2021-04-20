#include "Blinker.h"

Blinker _led1Blinker(2, 200);
Blinker _led2Blinker(5, 333);
Blinker _led3Blinker(9, 1111);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  _led1Blinker.update();
  _led2Blinker.update();
  _led3Blinker.update();
}
