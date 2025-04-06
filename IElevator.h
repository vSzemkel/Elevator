#pragma once

class IElevator
{
  public:
    virtual ~IElevator() {};
    virtual int GetCurrentFloor() const = 0;
    virtual bool IndoorRequest(const int floor) = 0;
    virtual bool OutdoorRequest(const int from, const bool requestedUp) = 0;
};
