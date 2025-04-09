
#include "Elevator.h"

Elevator::Elevator(const int floorsCount)
    : _floorsCount(floorsCount)
    , _observers(floorsCount)
{

}

std::unique_ptr<IElevator> Elevator::CreateElevator(const int floorsCount)
{
    return std::unique_ptr<IElevator>(new Elevator(floorsCount));
}

int Elevator::GetFloorsCount() const
{
    return _floorsCount;
}

int Elevator::GetCurrentFloor() const
{
    return _currentFloor;
}

bool Elevator::IndoorRequest(const int floor, notification_t callback)
{
    return true;
}

bool Elevator::OutdoorRequest(const int from, const bool requestedUp, notification_t callback)
{
    return true;
}

void Elevator::Move(const int targetFloor)
{
    _goingUp = _currentFloor < targetFloor;
    _currentFloor = targetFloor;

    for (auto& observer : _observers[targetFloor])
        observer();
}
