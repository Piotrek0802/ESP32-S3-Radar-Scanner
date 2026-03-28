#include <Wire.h>
#include <VL53L0X.h>

class laserVL53
{
private:
    VL53L0X sensor;
    uint16_t timeForLaser = 20;
    uint16_t timeoutLaser = 1000;

public:
    laserVL53(/* args */) {}
    ~laserVL53() {}
    void start()
    {
        sensor.setTimeout(timeoutLaser);
        if (sensor.init())
        {
            Serial.println("Poprawnie wlaczany laser");
        }
        else
        {
            Serial.println("Blad wlaczenia lasera");
        }
        sensor.setMeasurementTimingBudget(timeForLaser * 100);
    }
    int getReading()
    {
        int distance = sensor.readRangeSingleMillimeters();
        if (sensor.timeoutOccurred())
        {
            Serial.println("Timeout occured-laser");
            return 0;
        }
        return distance;
    }
    void setLaserRange(uint16_t x)
    {
        sensor.setMeasurementTimingBudget(x * 100);
        timeForLaser = x;
    }
    void setTimeForLaser(uint16_t x)
    {
        timeForLaser = x;
    }
    uint16_t getTimeForLaser()
    {
        return timeForLaser;
    }
    void setTimeoutLaser(uint16_t x)
    {
        timeoutLaser = x;
    }
    uint16_t getTimeoutLaser()
    {
        return timeoutLaser;
    }
};