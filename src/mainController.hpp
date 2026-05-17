#include <Arduino.h>
#include <Adafruit_NeoPixel.h> // do diody RGB na ESP32-S3;
#include <Adafruit_GFX.h>      //do ekranu OLED
#include <Adafruit_SSD1306.h>  //do ekranu OLED
#include <vector>              //wektory

#include "point.h"
#include "display.hpp"
#include "laserVL53.hpp"
#include "joystickController.hpp"
#include "status.h"
#include "servo.hpp"

Adafruit_NeoPixel pixels(1, 48, NEO_GRB + NEO_KHZ800); // do diody RGB na ESP32-S3 - NazwaObiektu(LiczbaDiod, Pin, TypSygnału)

class mainController
{
private:
    Display screen{};
    laserVL53 laser;
    joystickController *joystick = nullptr;
    servo *mySerwoX = nullptr; // lewo prawo
    servo *myServoY = nullptr; // gora dol
    // laser
    bool isLongRange = false;
    // zmienne do: RADAR
    std::vector<uint16_t> readings;
    // do obliczen itp "mozg"
    uint8_t resolution = 51;
    uint8_t textSize = 1;
    uint8_t radarRadius = 53;
    uint8_t servoYAngle = 90;
    int8_t maxRadarLeftRight = 45;
    uint16_t maxRead = 2000;
    uint16_t autoRadarPeriod = 40000;
    double angleX = 90, angleY = 90;
    bool isAutomaticScanning = false;
    bool isServoYOn = false;
    Point a{0, 0}, b{0, 0};

    // joystick
    double sensitivity = 0000.1;
    long long int lastTime = millis();

    uint8_t menuTextSize = 1;

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
        ESP32PWM::allocateTimer(0);
        mySerwoX = new servo(11);
        myServoY = new servo(12);
    }
    // wszystkie testy dzialaly na poprzednich wersjach klas .hpp
    // chyba na tym: 1c54850f2125bc72c5ad1f648e8738040f8f790d
    // obecnie nie korzystac!
    // pozniej moze naprawie
    /*
    void testCircle(uint16_t x = 25)
    {
        if (x > 50)
        {
            x = 50;
        }

        for (size_t i = 0; i < x; i++)
        {
            screen.printRadar(64 - 26 + i);
            screen.printToDisplay(DefaultPoints::CenterDown.withOffset(0, -10), 64 - 26 + i);
            screen.printToDisplay(DefaultPoints::LeftTop, i);
            screen.printToDisplay(DefaultPoints::RightTop.withOffset(-18, 0), 100 - i);
            delay(1000);
        }
    }
        */
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
            screen.clear();
            char str[3]{'0', '0', '0'};
            screen.printToDisplay(point, i);
            str[2] = static_cast<char>(i + 'A');
            screen.printToDisplay(point1, str);
            screen.show();
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
            screen.clear();
            screen.printToDisplay(DefaultPoints::RightTop.withOffset(-10), laser.getError());
            laser.setLongRange(true);
            screen.setTextSize(2);
            screen.printToDisplay(DefaultPoints::Center, laser.getReading());
            screen.show();

            delay(200);
        }
    }
    void testJoystick()
    {
        joystick = new joystickController(4, 5, 6);
        // screen.setTextSize(2);
        while (true) // joystick.isPressed() == false)
        {
            joystick->update();
            // screen.printToDisplay(DefaultPoints::CenterLeft, joystick->getInputX());
            // screen.printToDisplay(DefaultPoints::Center, joystick->getInputY(),);
            Serial.print("Joystick.X: ");
            Serial.println(joystick->getInputX());
            Serial.print("Joystick.Y: ");
            Serial.println(joystick->getInputY());

            if (joystick->isDoubleClicked())
            {
                joystick->setDefaultAxes();
            }
            delay(50);
        }

        // screen.setTextSize(3);
        // screen.updateTextSize();
        // screen.printToDisplay(DefaultPoints::Center.withOffset(-62), "Pressed");
        // screen.setTextSize(1);
        // screen.updateTextSize();
        // delay(5000);
    }

    void radar()
    {
        joystick = new joystickController(5, 4, 6); // ustawienia na ktorym pinie jest osX/Y i button

        joystick->setDefaultAxes();

        readings.resize(resolution);

        while (true)
        {

            // ---GUI---
            // 6x8 pikseli
            screen.clear();
            whiteTextBackground(false);
            screen.setTextSize(textSize);
            screen.printToDisplay(DefaultPoints::LeftTop, "Angle: ");
            screen.printToDisplay(DefaultPoints::LeftTop.withOffset(6 * 6 * textSize), angleX);
            screen.printRadar(radarRadius, DefaultPoints::CenterDown);
            screen.printToDisplay(DefaultPoints::RightTop.withOffset(-6 * 9), "mm: ");
            screen.printToDisplay(DefaultPoints::RightTop.withOffset(-6 * 4), int(readings[map(angleX, 0, 180, 0, resolution - 1)]));
            // do testow joysticka mozna pozniej dac w komentarz
            screen.printToDisplay(DefaultPoints::LeftTop.withOffset(0, 8), angleX);
            screen.printToDisplay(DefaultPoints::LeftTop.withOffset(0, 16), angleY);

            // rysowanie linii na radarze
            for (uint8_t i = 1; i < resolution; i++)
            {
                a.x = map(readings.at(i) * cos(double(i) / resolution * PI), -maxRead, maxRead, -radarRadius, radarRadius) + DefaultPoints::CenterDown.x;
                a.y = DefaultPoints::CenterDown.y - map(readings.at(i) * sin(double(i) / resolution * PI), 0, maxRead, 0, radarRadius);
                b.x = map(readings.at(i) * cos(double(i - 1) / resolution * PI), -maxRead, maxRead, -radarRadius, radarRadius) + DefaultPoints::CenterDown.x;
                b.y = DefaultPoints::CenterDown.y - map(readings.at(i - 1) * sin(double(i - 1) / resolution * PI), 0, maxRead, 0, radarRadius);
                screen.printTriangle(a, b, DefaultPoints::CenterDown);
            }

            screen.show();

            // ---RADAR---
            laser.setLongRange(false);
            readings.at(int(map(angleX, 0, 180, 0, resolution - 1))) = constrain(laser.getReading(), 0, maxRead);

            // ---JOYSTICK---
            joystick->update();
            if (isAutomaticScanning)
            {
                angleX = linearWave(millis(), autoRadarPeriod, 180);
                angleX = map(angleX, 0, 180, 90 - maxRadarLeftRight, 90 + maxRadarLeftRight);
                mySerwoX->setAngle(angleX); // <---AUTO RADAR---
                myServoY->setAngle(servoYAngle);
            }
            else
            {
                angleX += (int(joystick->getInputX()) - 2048) * (millis() - lastTime) * sensitivity / 1000;
                angleY += (int(joystick->getInputY()) - 2048) * (millis() - lastTime) * sensitivity / 1000;
                // ustawienie limitow obrotu
                angleX = constrain(angleX, 0, 180);
                angleY = constrain(angleY, 0, 180);
            }
            lastTime = millis();

            if (joystick->isPressed())
            {
                menu();
            }

            delay(20); // chwila oddechu
        }
    }

    void menu()
    {
        const uint8_t maxChoice = 9;
        const char *opcjeMenu[maxChoice] = {
            "1. Wyjdz",
            "2. Joystick calibration",
            "3. Laser long range",
            "4. Automatic scanning",
            "5. Ustaw serwoY",
            "6. Reset pamieci",
            "7. Max radar angle 90+-X",
            "8. Opcja 8",
            "9. Opcja 9"};
        int8_t choice = 0;
        int8_t firstVisibleLine = 0;
        const uint8_t maxVisibleLines = 6;

        bool wasUpOrDown = false;

        screen.setTextSize(menuTextSize);

        delay(200);

        while (true)
        {
            //---JOYSTICK---
            joystick->update();
            uint16_t joyStickY = joystick->getInputY();

            if (joyStickY > 4096 * 0.8 && !wasUpOrDown)
            {
                if (choice > 0)
                    choice--;
                wasUpOrDown = true;
            }
            else if (joyStickY < 4096 * 0.2 && !wasUpOrDown)
            {
                if (choice < maxChoice - 1)
                    choice++;
                wasUpOrDown = true;
            }
            else if (joyStickY < 4096 * 0.7 && joyStickY > 4096 * 0.3)
            {
                wasUpOrDown = false;
            }

            if (choice < firstVisibleLine)
            {
                firstVisibleLine = choice;
            }
            if (choice >= firstVisibleLine + maxVisibleLines)
            {
                firstVisibleLine = choice - maxVisibleLines + 1;
            }

            // ---GUI---
            screen.clear();

            for (uint8_t i = 0; i < maxVisibleLines; i++)
            {
                uint8_t currentMenuIndex = firstVisibleLine + i;
                if (currentMenuIndex >= maxChoice)
                    break;

                whiteTextBackground(choice == currentMenuIndex);

                screen.printToDisplay(DefaultPoints::LeftTop.withOffset(0, 10 * i), opcjeMenu[currentMenuIndex]);
            }

            whiteTextBackground(false);
            screen.show();

            if (joystick->isPressed())
            {
                delay(200);

                switch (choice + 1)
                {
                case 1:
                    return;

                case 2:
                    screen.clear();
                    screen.printToDisplay(DefaultPoints::CenterLeft, "Nie dotykaj! Kalibracja...");
                    screen.show();
                    joystick->setDefaultAxes();
                    screen.clear();
                    screen.printToDisplay(DefaultPoints::CenterLeft, "Skalibrowano!");
                    screen.show();
                    delay(1500);
                    break;

                case 3:
                    isLongRange = !isLongRange;
                    laser.setLongRange(isLongRange);
                    screen.clear();
                    if (isLongRange)
                    {
                        screen.printToDisplay(DefaultPoints::CenterLeft, "Long range: ON");
                    }
                    else
                    {
                        screen.printToDisplay(DefaultPoints::CenterLeft, "Long range: OFF");
                    }
                    screen.show();
                    delay(1500);
                    break;
                case 4:
                    isAutomaticScanning = !isAutomaticScanning;
                    screen.clear();
                    if (isAutomaticScanning)
                    {
                        screen.printToDisplay(DefaultPoints::CenterLeft, "Automatic scanning:   ON");
                    }
                    else
                    {
                        screen.printToDisplay(DefaultPoints::CenterLeft, "Automatic scanning:   OFF");
                    }
                    screen.show();
                    delay(1500);
                    break;
                case 5:
                    do
                    {
                        screen.clear();
                        screen.printToDisplay(DefaultPoints::CenterLeft, servoYAngle);
                        screen.show();
                        joystick->update();
                        servoYAngle += ((joystick->getInputY()) - 2048) * (millis() - lastTime) * sensitivity / 5000;
                        lastTime = millis();
                        servoYAngle = constrain(servoYAngle, 0, 180);
                        myServoY->setAngle(servoYAngle);
                    } while (joystick->isPressed() == false);
                    choice = 0;
                    break;
                case 6:
                    readings.clear();
                    screen.clear();
                    screen.printToDisplay(DefaultPoints::CenterLeft, "Wyczysczono pamiec");
                    screen.show();
                    delay(1500);
                    break;
                case 7:
                    do
                    {
                        screen.clear();
                        screen.printToDisplay(DefaultPoints::CenterLeft, "Obrot 90*+- ");
                        screen.printToDisplay(DefaultPoints::CenterLeft.withOffset(12 * 6), maxRadarLeftRight);
                        screen.show();

                        joystick->update();
                        maxRadarLeftRight += (joystick->getInputY()-2048) * sensitivity / 1000 * (millis() - lastTime);
                        maxRadarLeftRight = constrain(maxRadarLeftRight, 0, 90);
                        lastTime = millis();
                    } while (joystick->isPressed() == false);
                    delay(350);
                    break;

                default:
                    break;
                }
            }

            delay(30); // Mały odpoczynek dla procesora
        }
    }
    void whiteTextBackground(bool force = false)
    {
        if (force)
        {
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        }
        else
        {
            display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
        }
    }
    double linearWave(double x, double period, double maxVal)
    {
        // x - wejście (np. czas, kąt, indeks)
        // period - co ile "kroków" fala ma się powtarzać
        // maxVal - maksymalna wartość, jaką fala osiągnie

        double modulo = fmod(x, period);
        double halfPeriod = period / 2;

        return maxVal * (1.0 - abs(modulo - halfPeriod) / halfPeriod);
    }
};