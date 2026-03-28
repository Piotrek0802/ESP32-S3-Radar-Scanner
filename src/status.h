#include <cstdint>

// FUTURE: use this for errors

enum class Status
{
    NONE,
    LASER_READ,
    SERVO,
    SCREEN,
    CRITICAL,
};

struct SystemStatus
{
    Status error = Status::NONE;
    uint16_t lastUpdate = 0;
    bool isStale = false;
    uint16_t staleTime = 1000;
};