
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

bool Elevator::IndoorRequest(const int targetFloor, notification_t callback)
{
    const request_t request{callback, targetFloor, direction_e::INSIDE };

    std::unique_lock lock{_mutex};
    _receiving_queue.push_back(request);
    lock.unlock();
    _condv.notify_one();

    return true;
}

bool Elevator::OutdoorRequest(const int fromFloor, const bool requestedUp, notification_t callback)
{
    const request_t request{callback, fromFloor, requestedUp ? direction_e::UP : direction_e::DOWN};

    std::unique_lock lock{_mutex};
    _receiving_queue.push_back(request);
    lock.unlock();
    _condv.notify_one();

    return true;
}

void Elevator::Move(const int targetFloor)
{
    _goingUp = _currentFloor < targetFloor;
    _currentFloor = targetFloor;

    for (auto& observer : _observers[targetFloor])
        observer();
}
