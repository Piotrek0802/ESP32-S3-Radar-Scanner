#include <Arduino.h>

class joystickController
{
private:
    const uint8_t pinX, pinY, pinButton;
    uint16_t _deadZoneX = 125;
    uint16_t _deadZoneY = 125;
    uint16_t _deadZoneXmax = 0;
    uint16_t _deadZoneYmax = 0;
    uint16_t _inputX = 0;
    uint16_t _inputY = 0;
    int32_t _defaultX = 2048;
    int32_t _defaultY = 2048;
    bool _isPressed = false;
    bool _isButtonDoubleClicked = false;

    unsigned long lastButtonPressed;

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
    uint16_t getInputX() const
    {
        Serial.print("Joystick.X: ");
        Serial.println(_inputX);
        return _inputX;
    }
    uint16_t getInputY() const
    {
        Serial.print("Joystick.Y: ");
        Serial.println(_inputY);
        return _inputY;
    }
    bool isPressed() const { return _isPressed; }
    bool isDoubleClicked() const { return _isButtonDoubleClicked; }

    // --- Settery (z prostą walidacją dla bezpieczeństwa) ---
    void setDeadZoneX(uint16_t val) { _deadZoneX = val; }
    void setDeadZoneY(uint16_t val) { _deadZoneY = val; }

    // Settery dla wejść (używane przez funkcję odczytującą piny)
    void setInputX(int32_t val)
    {
        if (abs(val - _defaultX) < _deadZoneX)
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
            if (val <= _defaultX)
            {
                _inputX = map(val, 0, _defaultX, _deadZoneXmax, 2048);
            }
            else
            {
                _inputX = map(val, _defaultX, 4095, 2048 + _deadZoneX, 4095 - _deadZoneXmax);
            }
        }
        // Serial.print("JoystickController.X: ");
        // Serial.println(val+"  "+_inputX);
    }
    void setInputY(int32_t val)
    {
        if (abs(val - _defaultY) < _deadZoneY)
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
            if (val <= _defaultY)
            {
                _inputY = map(val, 0, _defaultY, _deadZoneYmax, 2048);
            }
            else
            {
                _inputY = map(val, _defaultY, 4095, 2048 + _deadZoneY, 4095 - _deadZoneYmax);
            }
        }
        // Serial.print("JoystickController.Y: ");
        // Serial.println(val+"  "+_inputY);
    }
    void setIsPressed(bool state)
    {
        _isPressed = state;
        _isButtonDoubleClicked = false;
        if (state)
        {
            if (millis() - lastButtonPressed < 750 && millis() - lastButtonPressed > 100)
            {
                _isButtonDoubleClicked = true;
            }
            lastButtonPressed = millis();
        }
    }
    void setDefaultAxes()
    {
        long int x = millis();
        while (millis() - x < 2500)
        {
            /* code */
        }
        _defaultX = analogRead(pinX);
        _defaultY = analogRead(pinY);
    }
};