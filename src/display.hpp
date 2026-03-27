#include <Arduino.h>
#include <Adafruit_GFX.h>     //do ekranu OLED
#include <Adafruit_SSD1306.h> //do ekranu OLED

#include "point.h" //punkt - struct

constexpr uint16_t dispWidth = 128;
constexpr uint16_t dispHeight = 64;

Adafruit_SSD1306 display(dispWidth, dispHeight, &Wire, -1);

class Display
{
private:
    Point leftUp, rightUp, centerDown;
    uint8_t textSize = 1, realTextSize;

public:
    Display(/* args */)
    {
    }
    ~Display() {}

    void start()
    {
        if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
        {
            Serial.println("Poprawnie wlaczany ekran");
        }
        else
        {
            Serial.println("Blad wlaczenia ekranu");
        }
        display.setTextColor(SSD1306_WHITE);
        display.clearDisplay();
        display.display();

        leftUp.x = 0;
        leftUp.y = 0;
        rightUp.x = dispWidth - 10;
        rightUp.y = 0;
        centerDown.x = dispWidth / 2;
        centerDown.y = dispHeight;
        updateTextSize();
    }
    void printToDisplay(Point x, char str[8], bool clean = true)
    {
        if (clean)
        {
            display.clearDisplay();
        }
        display.setCursor(x.x, x.y);
        display.println(str);
        display.display();
    }
    void printToDisplay(Point x, int str, bool clean = true)
    {
        if (clean)
        {
            display.clearDisplay();
        }
        display.setCursor(x.x, x.y);
        display.println(str);
        display.display();
    }
    void setTextSize(uint8_t x)
    {
        textSize = x;
    }
    uint8_t getTextSize()
    {
        return textSize;
    }
    void setRealTextSize(uint8_t x)
    {
        realTextSize = x;
    }
    uint8_t getRealTextSize()
    {
        return realTextSize;
    }
    void updateTextSize()
    {
        display.setTextSize(textSize);
        realTextSize = textSize;
    }
};