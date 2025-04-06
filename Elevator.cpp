
#include "Elevator.h"

Elevator::Elevator(const int floors)
{

}

std::unique_ptr<IElevator> Elevator::CreateElevator(const int floors)
{
    return std::unique_ptr<IElevator>(new Elevator(floors));
}

int Elevator::GetFloorsCount() const
{
    return 0;
}

int Elevator::GetCurrentFloor() const
{
    return _floor;
}

bool Elevator::IndoorRequest(const int floor, notification_t callback)
{
    return true;
}

bool Elevator::OutdoorRequest(const int from, const bool requestedUp, notification_t callback)
{
    return true;
}
