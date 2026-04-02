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
    void setLongRange(bool enable)
    {
        if (enable)
        {
            //max current scan distance ~1,8m-slow
            //max max scan distance ~2m-very slow sensor.setSignalRateLimit(0.05);sensor.setMeasurementTimingBudget(1000000);
            sensor.setSignalRateLimit(0.065);
            sensor.setMeasurementTimingBudget(600000);
            sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
            sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
        }
        else
        {
            sensor.setSignalRateLimit(0.25);
            sensor.setMeasurementTimingBudget(timeForLaser);
            sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 14);
            sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 10);
        }
    }
    uint8_t getError()
    {
        return sensor.last_status;
    }
    void setTimeForLaser(uint16_t x)
    {
        timeForLaser = x;
        sensor.setMeasurementTimingBudget(x);
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