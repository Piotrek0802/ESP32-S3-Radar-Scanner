#include <ESP32Servo.h>

class servo
{
private:
    Servo myServo;
    const uint8_t _servoPin;
    uint8_t _currentAngle = 90;
    uint8_t _maxAngle = 180;
    uint8_t _minAngle = 0;
    uint8_t _centerAngle = 0;

public:
    servo(uint8_t servoPin) : _servoPin(servoPin)
    {
        myServo.setPeriodHertz(50); // ile Hz - (standard 50Hz)
        myServo.attach(_servoPin, 500, 2400);
    }
    ~servo() {}

    // gettery i settery
    uint8_t getCurrentAngle() const { return _currentAngle; }
    void setCurrentAngle(uint8_t angle) { _currentAngle = angle; }

    uint8_t getMaxAngle() const { return _maxAngle; }
    void setMaxAngle(uint8_t angle) { _maxAngle = angle; }

    uint8_t getMinAngle() const { return _minAngle; }
    void setMinAngle(uint8_t angle) { _minAngle = angle; }

    uint8_t getCenterAngle() const { return _centerAngle; }
    void setCenterAngle(uint8_t angle) { _centerAngle = angle; }

    // metody
    void setAngle(uint8_t angle)
    {
        myServo.write(angle);
        _currentAngle = angle;
    }
    void setNewCenterAngle()
    {
        _centerAngle = _currentAngle;
    }
    void setCenterAngle()
    {
        myServo.write(_centerAngle);
    }
    void setPlusRight(uint8_t angle)
    {
        myServo.write(constrain(_currentAngle + angle,0,180));
    }
    void setPlusLeft(uint8_t angle)
    {
        myServo.write(constrain(_currentAngle - angle,0,180));
    }
};