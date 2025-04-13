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
    Elevator(const int floorsCount) noexcept;
    int GetFloorsCount() const override;
    int GetCurrentFloor() const override;
    bool IndoorRequest(const int targetFloor, notification_t callback) override;
    bool OutdoorRequest(const int fromFloor, const bool requestedUp, notification_t callback) override;

    static std::unique_ptr<IElevator> CreateElevator(const int floorsCount);

  private:
    void RequestLoop();
    void MoveLoop();
    void ProcessRequest(request_t& request);
    int SelectNextTargetFloor();
    void Move(const int targetFloor);

    std::priority_queue<int, std::vector<int>, std::greater<>> _requestsUp[2];
    std::priority_queue<int, std::vector<int>, std::less<>> _requestsDown[2];
    std::vector<request_t> _receiving_queue, _processed_queue;
    std::vector<std::vector<notification_t>> _observers;
    std::atomic<direction_e> _direction{};
    std::atomic<bool> _stop_requested{};
    std::atomic<int> _currentFloor{};
    std::condition_variable _req_condv;
    std::condition_variable _mov_condv;
    std::jthread _req_worker;
    std::jthread _mov_worker;
    std::mutex _req_mutex;
    std::mutex _mov_mutex;
    int _floorsCount{};
};
