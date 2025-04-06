
#include "ElevatorUser.h"
#include "IElevator.h"

ElevatorUser::ElevatorUser(IElevator& elevator, const int initFloor, const int targetFloor)
    : _targetFloor{targetFloor}
{
    elevator.OutdoorRequest(initFloor, initFloor < targetFloor);
}
