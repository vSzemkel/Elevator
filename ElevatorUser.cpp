
#include "ElevatorUser.h"
#include "IElevator.h"

#include <iostream>
#include <format>

ElevatorUser::ElevatorUser(IElevator& elevator, std::string name)
    : _elevator{elevator}
    , _name{std::move(name)}
{
}

bool ElevatorUser::CallElevator(const int initFloor, const int targetFloor)
{
    const auto floorsCount = _elevator.GetFloorsCount();
    if (initFloor < 0 || floorsCount <= initFloor || targetFloor < 0 || targetFloor <= initFloor || initFloor == targetFloor) {
        std::cerr << std::format("Invalid move from {} to {} requested by {}.\n", initFloor, targetFloor, _name);
        return false;
    }

    if (_insideCabin) {
        std::cerr << std::format("{} being inside cabin can't call elevator.\n", _name);
        return false;
    }

    _initFloor = initFloor;
    _targetFloor = targetFloor;

    auto callback = [&](){ this->EnterCabin(); };
    return _elevator.OutdoorRequest(initFloor, initFloor < targetFloor, std::move(callback));
}

void ElevatorUser::EnterCabin()
{
    if (_initFloor != _elevator.GetCurrentFloor()) {
        std::cerr << std::format("{} requested to enter cabin on wrong floor. Expected {}, actual {}.\n", _name, _initFloor, _elevator.GetCurrentFloor());
        return;
    }

    if (_insideCabin) {
        std::cerr << std::format("{} requested to enter cabin on wrong floor. Expected {}, actual {}.\n", _name, _initFloor, _elevator.GetCurrentFloor());
        return;
    }

    _insideCabin = true;
 
    auto callback = [&]() { this->ExitCabin(); };
    _elevator.IndoorRequest(_targetFloor, std::move(callback));
}

void ElevatorUser::ExitCabin()
{
    if (_targetFloor != _elevator.GetCurrentFloor()) {
        std::cerr << std::format("{} requested to exit cabin on wrong floor. Expected {}, actual {}.\n", _name, _targetFloor, _elevator.GetCurrentFloor());
        return;
    }

    _insideCabin = false;
}
