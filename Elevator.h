#pragma once

#include"IElevator.h"

#include <atomic>
#include <memory>
#include <queue>
#include <vector>

class Elevator : public IElevator
{
    using request_t = std::pair<int, notification_t>;

  public:
    Elevator(const int floorsCount);
    int GetFloorsCount() const override;
    int GetCurrentFloor() const override;
    bool IndoorRequest(const int floor, notification_t callback) override;
    bool OutdoorRequest(const int from, const bool requestedUp, notification_t callback) override;

    static std::unique_ptr<IElevator> CreateElevator(const int floorsCount);

  private:
    void Move(const int targetFloor);

    std::priority_queue<request_t, std::vector<request_t>, std::greater<>> _requestsUp[2];
    std::priority_queue<request_t, std::vector<request_t>, std::less<>> _requestsDown[2];
    std::vector<std::vector<notification_t>> _observers;
    std::queue<request_t> _actorQueue[2];
    std::atomic<int> _currentFloor{};
    int _floorsCount{};
    bool _goingUp{};
};
