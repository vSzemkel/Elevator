#pragma once

#include"IElevator.h"

#include <atomic>
#include <memory>
#include <queue>

class Elevator : public IElevator
{
    using request_t = std::pair<int, notification_t>;

  public:
    Elevator(const int floors);
    int GetFloorsCount() const override;
    int GetCurrentFloor() const override;
    bool IndoorRequest(const int floor, notification_t callback) override;
    bool OutdoorRequest(const int from, const bool requestedUp, notification_t callback) override;

    static std::unique_ptr<IElevator> CreateElevator(const int floor);

  private:
    std::priority_queue<request_t, std::vector<request_t>, std::greater<>> _requestsUp[2];
    std::priority_queue<request_t, std::vector<request_t>, std::less<>> _requestsDown[2];
    std::atomic<int> _floor{};
    bool _goingUp{};
};
