#include <Arduino.h>

class joystickController
{
private:
    const uint8_t pinX, pinY, pinButton;
    uint16_t _deadZoneX = 25;
    uint16_t _deadZoneY = 25;
    uint16_t _deadZoneXmax = 0;
    uint16_t _deadZoneYmax = 0;
    uint16_t _inputX = 0;
    uint16_t _inputY = 0;
    bool _isPressed = false;

public:
    joystickController(uint8_t pinX1, uint8_t pinY1, uint8_t pinButton1) : pinX(pinX1), pinY(pinY1), pinButton(pinButton1)
    {
        pinMode(pinX, ANALOG);
        pinMode(pinY, ANALOG);
        pinMode(pinButton, INPUT_PULLUP);
    }
    ~joystickController() {}

    void update()
    {
        setInputX(analogRead(pinX));
        setInputY(analogRead(pinY));
        setIsPressed(!digitalRead(pinButton));
    }

    uint16_t getDeadZoneX() const { return _deadZoneX; }
    uint16_t getDeadZoneY() const { return _deadZoneY; }
    uint16_t getDeadZoneXmax() const { return _deadZoneXmax; }
    uint16_t getDeadZoneYmax() const { return _deadZoneYmax; }
    uint16_t getInputX() const { return _inputX; }
    uint16_t getInputY() const { return _inputY; }
    bool isPressed() const { return _isPressed; }

    // --- Settery (z prostą walidacją dla bezpieczeństwa) ---
    void setDeadZoneX(uint16_t val) { _deadZoneX = val; }
    void setDeadZoneY(uint16_t val) { _deadZoneY = val; }

    // Settery dla wejść (używane przez funkcję odczytującą piny)
    void setInputX(int32_t val)
    {
        if (abs(val - 2048) < _deadZoneX)
        {
            _inputX = 2048;
        }
        else if (val > 4095 - _deadZoneXmax)
        {
            _inputX = 4095;
        }
        else if (val < _deadZoneXmax)
        {
            _inputX = 0;
        }
        else
        {
            _inputX = val;
        }
        //Serial.print("JoystickController.X: ");
        //Serial.println(val+"  "+_inputX);
    }
    void setInputY(int32_t val)
    {
        if (abs(val - 2048) < _deadZoneY)
        {
            _inputY = 2048;
        }
        else if (val > 4095 - _deadZoneYmax)
        {
            _inputY = 4095;
        }
        else if (val < _deadZoneYmax)
        {
            _inputY = 0;
        }
        else
        {
            _inputY = val;
        }
        //Serial.print("JoystickController.Y: ");
        //Serial.println(val+"  "+_inputY);
    }

    void setIsPressed(bool state) { _isPressed = state; }
};