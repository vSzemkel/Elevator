
#include "Elevator.h"

Elevator::Elevator(const int floors)
{

}

std::unique_ptr<IElevator> Elevator::CreateElevator(const int floors)
{
    return std::unique_ptr<IElevator>(new Elevator(floors));
}

int Elevator::GetCurrentFloor() const
{
    return _floor;
}

bool Elevator::IndoorRequest(const int floor)
{
    return true;
}

bool Elevator::OutdoorRequest(const int from, const bool requestedUp)
{
    return true;
}
