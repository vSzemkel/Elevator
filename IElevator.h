#pragma once

#include <functional>

class IElevator
{
  public:
    using notification_t = std::function<void()>;

    virtual ~IElevator() {};
    virtual void RequestTermination() = 0;
    virtual int GetFloorsCount() const = 0;
    virtual int GetCurrentFloor() const = 0;
    virtual bool IndoorRequest(const int targetFloor, notification_t&& callback) = 0;
    virtual bool OutdoorRequest(const int fromFloor, const bool requestedUp, notification_t&& callback) = 0;
};
