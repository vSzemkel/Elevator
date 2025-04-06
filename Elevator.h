#pragma once

#include"IElevator.h"

#include <memory>
#include <queue>

class ElevatorUser;

class Elevator : public IElevator
{
    using request_t = std::pair<int, const ElevatorUser&>;

  public:
    Elevator(const int floors);
    int GetCurrentFloor() const override;
    bool IndoorRequest(const int floor) override;
    bool OutdoorRequest(const int from, const bool requestedUp) override;

    static std::unique_ptr<IElevator> CreateElevator(const int floor);

  private:
    int _floor{};
    bool _goingUp{};
    std::priority_queue<request_t, std::vector<request_t>, std::greater<>> _requestsUp[2];
    std::priority_queue<request_t, std::vector<request_t>, std::less<>> _requestsDown[2];
};
