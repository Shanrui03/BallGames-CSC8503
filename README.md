# BallGames-CSC8503
CSC8503 Project of Ball Games
# Technical list
  
  
|Category                |Feature                          |Description                         |
|----------------|-------------------------------|-----------------------------|
|Physics          |Raycasting            |Fires infinitely thin lines from a point in the world to see which objects collide with it.            |
|Physics          |Linear Motion            |Uses a semi-implicit Euler integrator to simulate rigid body dynamics.            |
|Physics          |Angular Motion      |Simulates angular movement dynamics using moments of inertia. |
|Physics          |Collision Detection+Resolution      |Sphere and AABB detection capabilities, with a plan to eventually implement OBB, capsule and swept volume collisions. |
|Physics          |Broad & Narrow Phase      |Octree Broad and Narrow spatial acceleration structure implementations.  |
|AI          |State Machine      |Using a templated generic FSM. |
|AI          |Pushdown Automata      |Implement a special state machine similar to stack. |
|AI          |Behaviour Tree     |Implementing behavioural trees of parallel, selective and sequential types. |
|AI          |A* Pathfinding     |Using Manhattan heuristic on grid-based paths. |


# Level 1
Level 1 is a display of physical and general state machine effects, containing collision detection and regular collision resolution between AABB, OBB, capsules, spheres and planes.Furthermore, Level 1 also demonstrates how well Raycasting detects various types of colliders and creates special colliders similar to Trigger types in Unity Engine, which can give the player special collision resolution.
[Level1](
