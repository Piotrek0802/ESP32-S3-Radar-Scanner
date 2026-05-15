#include <Arduino.h>
#include <WiFi.h>

#include "mainController.hpp"

mainController controller{};

void setup()
{
  Serial.begin(115200);
  Wire.begin(8, 9);
  controller.start();

  //wylaczenie wifi i bt dla zwolnienia zasobow i zmiejszenia poboru pradu
  WiFi.mode(WIFI_OFF);
  btStop();
}

void loop()
{
  //controller.testSimpleText(3); // test wypisywania tekstu na ekran
  //controller.testCircle(3); // test rysowania podstawowego radaru/dobierania rozmiaru okregu
  //controller.testLaserReading(500); //test odczytow lesera i wypisywania na ekran
  controller.testJoystick();
}
