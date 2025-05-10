
#include "pch.h"
#include "Elevator.h"

Elevator::Elevator(const int floorsCount) noexcept
    : _observers(floorsCount)
    , _floorRequested(floorsCount)
    , _req_worker([&]() { RequestLoop(); })
    , _mov_worker([&]() { MoveLoop(); })
    , _floorsCount(floorsCount)
{
}

std::unique_ptr<IElevator> Elevator::CreateElevator(const int floorsCount)
{
    return std::unique_ptr<IElevator>(new Elevator(floorsCount));
}

void Elevator::RequestTermination()
{
    while (_requestServed == 0 || _requestServed < _requestsToHandle)
        std::this_thread::sleep_for(std::chrono::seconds(1));

    _stop_requested = true;
    _req_condv.notify_one();
    _mov_condv.notify_one();
}

int Elevator::GetFloorsCount() const
{
    return _floorsCount;
}

int Elevator::GetCurrentFloor() const
{
    return _currentFloor;
}

bool Elevator::IndoorRequest(const int targetFloor, notification_t&& callback)
{
    // executed by elevator's moving thread, _req_mutex already owned
    _receiving_queue.emplace_back(targetFloor, direction_e::INSIDE);
    _observers[targetFloor].emplace_back(std::move(callback));

    return true;
}

bool Elevator::OutdoorRequest(const int fromFloor, const bool requestedUp, notification_t&& callback)
{
    std::unique_lock lock{_req_mutex};
    _receiving_queue.emplace_back(fromFloor, requestedUp ? direction_e::UP : direction_e::DOWN);
    _observers[fromFloor].emplace_back(std::move(callback));
    lock.unlock();
    _req_condv.notify_one();

    _requestsToHandle += 2;
    return true;
}

void Elevator::RequestLoop()
{
    while (!_stop_requested) {
        std::unique_lock lock{_req_mutex};
        while (_receiving_queue.empty() && !_stop_requested)
            _req_condv.wait(lock);

        std::swap(_receiving_queue, _processed_queue);
        lock.unlock();

        for (auto& r : _processed_queue)
            ProcessRequest(r);

        _processed_queue.clear();
    }
}

void Elevator::ProcessRequest(request_t& request)
{
    const auto& floor = request.floor;
    if (_floorRequested[floor].exchange(true))
        return;
    if (request.direction == direction_e::INSIDE)
        request.direction = (floor < _currentFloor) ? direction_e::DOWN : direction_e::UP;

    std::unique_lock lock{_mov_mutex};
    if (request.direction == direction_e::UP) {
        if (request.direction == _direction) {
            if (_currentFloor < floor)
                _requestsUp[0].push(floor);
            else
                _requestsUp[1].push(floor);
        } else
            _requestsUp[0].push(floor);
    } else {
        if (request.direction == _direction) {
            if (floor < _currentFloor)
                _requestsDown[0].push(floor);
            else
                _requestsDown[1].push(floor);
        } else
            _requestsDown[0].push(floor);
    }

    ++_requestQueueSize;
    lock.unlock();
    _mov_condv.notify_all();
}

void Elevator::MoveLoop()
{
    while (!_stop_requested) {
        std::unique_lock lock{_mov_mutex};
        while (_requestQueueSize == 0 && !_stop_requested)
            _mov_condv.wait(lock);

        --_requestQueueSize;
        const int nextFloor = SelectNextTargetFloor();
        lock.unlock();

        if (!_stop_requested)
            Move(nextFloor);
    }
}

int Elevator::SelectNextTargetFloor()
{
    const auto pop_return = [](auto& queue) { const auto top = queue.top(); queue.pop(); return top; };

    if (_stop_requested)
        return 0;

    if (_direction == direction_e::UP) {
        if (!_requestsUp[0].empty())
            return pop_return(_requestsUp[0]);
        else {
            std::swap(_requestsUp[0], _requestsUp[1]);
            if (!_requestsDown[0].empty())
                return pop_return(_requestsDown[0]);
            else {
                std::swap(_requestsDown[0], _requestsDown[1]);
                return SelectNextTargetFloor();
            }
        }
    } else {
        if (!_requestsDown[0].empty())
            return pop_return(_requestsDown[0]);
        else {
            std::swap(_requestsDown[0], _requestsDown[1]);
            if (!_requestsUp[0].empty())
                return pop_return(_requestsUp[0]);
            else {
                std::swap(_requestsUp[0], _requestsUp[1]);
                return SelectNextTargetFloor();
            }
        }
    }

    return 0;
}

void Elevator::Move(const int targetFloor)
{
    _direction = _currentFloor < targetFloor ? direction_e::UP : direction_e::DOWN;
    _currentFloor = targetFloor;
    std::cout << std::format("\033[32mElevator stops on {}.\033[0m\n", targetFloor);

    std::unique_lock lock{_req_mutex};
    auto& floorObservers = _observers[targetFloor];
    for (const auto& observer : floorObservers)
        observer();

    _floorRequested[targetFloor] = false;
    _requestServed += int(floorObservers.size());
    floorObservers.clear();
    lock.unlock();
    _req_condv.notify_one(); // handle inside requests
}
