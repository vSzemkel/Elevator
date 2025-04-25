
#include "pch.h"
#include "Elevator.h"
#include "ElevatorUser.h"

constinit const int FLOORS = 10;
constinit const int TOUR_LENGTH = 5;
constinit const int MAX_ERRAND_TIME = 100;

const auto rand_in_range = [](const int ubound) { std::random_device seed; std::mt19937 gen{seed()}; std::uniform_int_distribution<int> dist(0, ubound - 1); return dist(gen); };
static std::unique_ptr<IElevator> elevator;

void user_journey(const char* name)
{
    ElevatorUser user(*elevator, name);
    std::vector<int> tour(TOUR_LENGTH);
    std::generate_n(tour.begin(), TOUR_LENGTH, [] { return rand_in_range(FLOORS); });

    int prev{};
    for (const int cur : tour) {
        user.CallElevator(prev, cur);
        std::this_thread::sleep_for(std::chrono::milliseconds(rand_in_range(MAX_ERRAND_TIME)));
        prev = cur;
    }

    user.CallElevator(prev, 0);
    user.ExitBuilding();
}

int main()
{
    elevator = Elevator::CreateElevator(FLOORS);

    std::jthread eva{user_journey, "Eva"};
    std::jthread monica{user_journey, "Monica"};
    std::jthread sciana{user_journey, "Sciana"};
    std::jthread lotan{user_journey, "Lotan"};
    std::jthread eli{user_journey, "Elisheva"};

    elevator->RequestTermination();
    return 0;
}
