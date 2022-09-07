# BallGames-CSC8503
CSC8503 Project of Ball Games

# Youtube Link:
Most of the functionality is demonstrated in the following demo [video](https://youtu.be/wpxRXV3Y_FE)

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
![Level1](http://m.qpic.cn/psc?/6cbda8ec-3a34-4f2c-b58e-e699fa3b5e3c/bqQfVz5yrrGYSXMvKr.cqRE5V8OgVFVUzMzwQ6pJ9MaxK7haahfdTKb5uojGZZVtcDSy*UhM.BLhJznNDieFgLmzOowEhh2*lJWsip5ici8!/b&bo=nARTAwAAAAADB.o!&rf=viewer_4)

# Level 2
Level 2 is a specific maze generated from a Text file which will be used as a Navigation Grid to implement the A* pathfinding function.
<br>
[NavigationGrid](https://github.com/Shanrui03/BallGames-CSC8503/blob/main/Assets/Data/TestGrid1.txt)
<br>
Players need to take control of the ball to avoid enemies in the maze and reach to the end in a given time.There are some coins in the scene, pick them up to score points. In addition, there is a special bonus ball placed somewhere in the maze, and when the player approaches it to a certain distance, they can establish a constraint with the ball and earn a lot of points at the same time.
![Level2](http://m.qpic.cn/psc?/6cbda8ec-3a34-4f2c-b58e-e699fa3b5e3c/bqQfVz5yrrGYSXMvKr.cqXwA3IdMQ7s5hdO4MJd9oDSb.BsVhoqvAAmoVjp0OuoYgP07nigSEisE7Qs1ARg4kATRSjdfbpnq2QeJnsUOO0Y!/b&bo=mgRTAwAAAAADB.w!&rf=viewer_4)
<br>
Enemy AI is made up of a Selective Behaviour Tree. The enemy will Raycast by firing a ray forward at a fixed distance, thus simulating its field of vision. When the enemy does not "see" the player, it pursues the player by A* pathfinding. When the enemy "sees" coins, it prioritises picking up them, and when the enemy "sees" the player, it changes its pursuit from A* pathfinding to a straight-line pursuit.
![Raycast](http://m.qpic.cn/psc?/6cbda8ec-3a34-4f2c-b58e-e699fa3b5e3c/bqQfVz5yrrGYSXMvKr.cqccaDksNrk.EPPrmiiCF7kxCyVirS4cu880q1caH2xY8fzFslc9LxGsiGq*4dC5qMuMcnNbxDjKQo4GEAfKaRYw!/b&bo=oQRJAwAAAAADB80!&rf=viewer_4)
