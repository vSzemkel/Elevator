#pragma once

#include"IElevator.h"

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class Elevator : public IElevator
{
    enum class direction_e {
        DOWN,
        INSIDE,
        UP
    };

    struct request_t {
        notification_t callback;
        int floor;
        direction_e direction;
    };

  public:
    Elevator(const int floorsCount);
    int GetFloorsCount() const override;
    int GetCurrentFloor() const override;
    bool IndoorRequest(const int targetFloor, notification_t callback) override;
    bool OutdoorRequest(const int fromFloor, const bool requestedUp, notification_t callback) override;

    static std::unique_ptr<IElevator> CreateElevator(const int floorsCount);

  private:
    void Move(const int targetFloor);

    std::priority_queue<int, std::vector<int>, std::greater<>> _requestsUp[2];
    std::priority_queue<int, std::vector<int>, std::less<>> _requestsDown[2];
    std::vector<request_t> _receiving_queue, _processed_queue;
    std::vector<std::vector<notification_t>> _observers;
    std::atomic<int> _currentFloor{};
    std::condition_variable _condv;
    std::jthread _worker;
    std::mutex _mutex;
    int _floorsCount{};
    bool _goingUp{};
};
