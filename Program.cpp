
#include "pch.h"
#include "Elevator.h"
#include "ElevatorUser.h"

int main()
{
    auto elevator = Elevator::CreateElevator(20);
    ElevatorUser eva(*elevator, "Eva");
    eva.CallElevator(0, 7);
    ElevatorUser sciana(*elevator, "Sciana");
    sciana.CallElevator(3, 8);
    eva.CallElevator(7, 0);

    elevator->RequestTermination(6);
    return 0;
}
