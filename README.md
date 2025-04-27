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
- Elevator moving thread is notified with condition variable when request in some queue is available. Taking into account curret floor and direction it selects the best next target floor and schedules elevator move.
- Wait on atomic is used to prevent ElevatorUser to call elevator twice before being served.
- Request queues are populated in Elevator::ProcessRequest. Requests are taken from the queues by Elevator::SelectNextTargetFloor.
- Requests are idempotent. Requested floors state is preserved in _floorsRequested collection of atomics bool.
- Observers collection is populated in client thread for outer requests and cleared in Elevator::Move by elevator's moving thread. It is synchronized with request mutex.
- Access to queues is synchronized with mutex/conditional variable.
- Access to elevator's primitive attributes is synchronized with std::atomic.
- To prevent premature ElevatorUser destruction, explicit method ExitBuilding is to be called at interaction end.

<p>
<div style="display: flex; justify-content: space-around;">
  <img src="https://github.com/vSzemkel/Elevator/blob/main/UML/elevator-sequence.png" alt="Sequence diagram" width="50%">
  <img src="https://github.com/vSzemkel/Elevator/blob/main/UML/elevator-classes.png" alt="Class hierarchy" width="70%">
</div>
</p>