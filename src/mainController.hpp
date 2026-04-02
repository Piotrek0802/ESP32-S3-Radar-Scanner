#include <Arduino.h>
#include <Adafruit_NeoPixel.h> // do diody RGB na ESP32-S3;
#include <Adafruit_GFX.h>      //do ekranu OLED
#include <Adafruit_SSD1306.h>  //do ekranu OLED

#include "point.h"
#include "display.hpp"
#include "laserVL53.hpp"
#include "joystickController.hpp"
#include "status.h"

Adafruit_NeoPixel pixels(1, 48, NEO_GRB + NEO_KHZ800); // do diody RGB na ESP32-S3 - NazwaObiektu(LiczbaDiod, Pin, TypSygnału)

class mainController
{
private:
    Display screen{};
    laserVL53 laser;

public:
    mainController(/* args */)
    {
    }
    ~mainController()
    {
    }
    void start()
    {
        screen.start();
        laser.start();
    }
    void testCircle(uint16_t x = 25)
    {
        if (x > 50)
        {
            x = 50;
        }

        for (size_t i = 0; i < x; i++)
        {
            screen.printRadar(64 - 26 + i);
            screen.printToDisplay(DefaultPoints::CenterDown.withOffset(0, -10), 64 - 26 + i, false);
            screen.printToDisplay(DefaultPoints::LeftTop, i, false);
            screen.printToDisplay(DefaultPoints::RightTop.withOffset(-18, 0), 100 - i, false);
            delay(1000);
        }
    }
    void testSimpleText(uint16_t x = 26)
    {
        if (x > 26)
        {
            x = 26;
        }

        Point point, point1;
        point1.y = 10;
        for (int i = 0; i < x; i++)
        {
            char str[3]{'0', '0', '0'};
            screen.printToDisplay(point, i, true);
            str[2] = static_cast<char>(i + 'A');
            screen.printToDisplay(point1, str, false);
            delay(750);
        }
    }
    void testLaserReading(uint16_t x = 10)
    {
        if (x > 500)
        {
            x = 500;
        }
        for (size_t i = 0; i < x; i++)
        {
            screen.setTextSize(1);
            screen.updateTextSize();
            screen.printToDisplay(DefaultPoints::LeftTop, true, true);
            screen.printToDisplay(DefaultPoints::RightTop.withOffset(-10), laser.getError(), false);
            laser.setLongRange(true);
            screen.setTextSize(2);
            screen.updateTextSize();
            screen.printToDisplay(DefaultPoints::Center, laser.getReading(), false);
            delay(200);
        }
    }
    void testJoystick()
    {
        joystickController joystick(6, 5, 4);
        screen.setTextSize(2);
        screen.updateTextSize();
        while (joystick.isPressed() == false)
        {
            joystick.update();
            screen.printToDisplay(DefaultPoints::CenterLeft, joystick.getInputX(), true);
            screen.printToDisplay(DefaultPoints::Center, joystick.getInputY(), false);
            Serial.print("Joystick.X: ");
            Serial.println(joystick.getInputX());
            Serial.print("Joystick.Y: ");
            Serial.println(joystick.getInputY());
            delay(50);
        }
        screen.setTextSize(3);
        screen.updateTextSize();
        screen.printToDisplay(DefaultPoints::Center.withOffset(-62), "Pressed");
        screen.setTextSize(1);
        screen.updateTextSize();
        delay(5000);
    }
};