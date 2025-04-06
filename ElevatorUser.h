#pragma once

class IElevator;

class ElevatorUser
{
  public:
    ElevatorUser(IElevator& elevator, const int initFloor, const int targetFloor);

  private:
    int _targetFloor;
};
