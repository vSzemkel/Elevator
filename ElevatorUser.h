#pragma once

#include <string>

class IElevator;

class ElevatorUser
{
  public:
    ElevatorUser(IElevator& elevator, std::string name);
    bool CallElevator(const int initFloor, const int targetFloor);
    void EnterCabin();
    void ExitCabin();

  private:
    IElevator& _elevator;
    std::string _name;
    int _initFloor{-1};
    int _targetFloor{-1};
    bool _insideCabin{false};
};
