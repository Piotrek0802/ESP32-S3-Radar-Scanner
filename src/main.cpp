#include <Arduino.h>
#include <Adafruit_NeoPixel.h> // do diody RGB na ESP32-S3;
#include <Adafruit_GFX.h>      //do ekranu OLED
#include <Adafruit_SSD1306.h>  //do ekranu OLED

#include "point.h"
#include "display.hpp"

Adafruit_NeoPixel pixels(1, 48, NEO_GRB + NEO_KHZ800); // do diody RGB na ESP32-S3 - NazwaObiektu(LiczbaDiod, Pin, TypSygnału)

Point point, point1;

Display screen{};

void setup()
{
  Serial.begin(115200);

  screen.start();

  point1.y = 10;
}

void loop()
{
  for (int i = 0; i < 26; i++)
  {
    char str[3]{'0','0','0'};
    screen.printToDisplay(point, i, true);
    str[2]=static_cast<char>(i + 'A');
    screen.printToDisplay(point1, str, false);
    delay(750);
  }
}
