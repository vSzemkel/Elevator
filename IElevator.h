#pragma once

#include <functional>

class IElevator
{
  public:
    using notification_t = std::function<void()>;

    virtual ~IElevator() {};
    virtual int GetFloorsCount() const = 0;
    virtual int GetCurrentFloor() const = 0;
    virtual bool IndoorRequest(const int floor, notification_t callback) = 0;
    virtual bool OutdoorRequest(const int from, const bool requestedUp, notification_t callback) = 0;
};
