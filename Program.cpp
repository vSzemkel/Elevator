
#include <iostream>

#include "Elevator.h"
#include "ElevatorUser.h"

int main()
{
    auto elevator = Elevator::CreateElevator(20);
    ElevatorUser eva(*elevator, "Eva");
    eva.CallElevator(0, 7);

    return 0;
}
