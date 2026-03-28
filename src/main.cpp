#include <Arduino.h>

#include "mainController.hpp"

mainController controller{};

void setup()
{
  Serial.begin(115200);
  Wire.begin(8, 9);
  controller.start();
}

void loop()
{
  controller.testSimpleText(3);
  controller.testCircle(3);
  //controller.testLaserReading(500);//need to fix
  controller.testJoystick();
}
