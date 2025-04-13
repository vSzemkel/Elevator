# Design document for multithreaded elevator simulator

## Requests
- Person outside elevator can call it by pressing UP or DOWN button
- After entering the cabin she pushes target floor button

## Notifications
- Person outside is notified when she can enter the elevator 
- After arriving to target floor, elevator notifies person inside the cabin she can leave

## Threading model
Elevator is implemented with two threads. 

Request processing thread is responsible for taking requests from passangers and putting them in relevant request queue.

Elevator moving thread is responsible for determination current elevator's direction and target floor.

## Implementation details
- Requests are stored in 4 priority queues. Two for requests up and two for down. If request up comes when elevator goes up but is already higher then passanger, request cannot be server before elevator turns around. That's why it is stored in secondary up-queue. Situation is similar for down requests.
- Elevator moving thread is notified with condition variable when request in some queue is available. Taking into account curret floor and direction it selects the best next target floor and schedules elevator move
- Request queues are populated in Elevator::ProcessRequest. Requests are taken from the queues by Elevator::SelectNextTargetFloor.
- Access to queues is synchronized with mutex/conditional variable
- Access to elevator's primitive attributes is synchronized with std::atomic

