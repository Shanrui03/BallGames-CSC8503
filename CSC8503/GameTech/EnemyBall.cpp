#include "EnemyBall.h"
#include "../../Common/Maths.h"
#include "../GameTech/TutorialGame.h"
#include "../CSC8503Common/Debug.h"
#include "../CSC8503Common/CollisionDetection.h"
#include "../CSC8503Common/GameWorld.h"
//basic functions
void EnemyBall::Respawn()
{
	transform.SetPosition(Vector3(70, 5, 0));
	player->GetTransform().SetPosition(Vector3(30, 10, 120));
	if (constraintSphere->statename == "Build Constraint!")
	{
		constraintSphere->GetTransform().SetPosition(Vector3(35, 10, 120));
	}
}
void EnemyBall::InitBehaviourTree()
{
	BehaviourRoot = new BehaviourSequence("Move");

	BehaviourAction* MoveToDest = new BehaviourAction("Move to Player",
		[&](float dt, BehaviourState state) -> BehaviourState {
			//Initialise state action
			if (state == Initialise) 
			{
				if (relativeDistance <= startrelativeDistance)
				{
					this->statename = "Chasing player!";
					state = Ongoing;
				}
				else 
				{
					this->statename = "Waiting for player..";
				}
			}

			//Ongoing state action
			else if (state == Ongoing) 
			{
				machine->Update(dt);
				if (relativeDistance <= 5.0f)
				{
					state = Success;
				}
			}

			//Success state action
			else if (state == Success) 
			{
				this->statename = "Success";
				Respawn();
				state = Initialise;
			}

			return state;
		}
	);

	BehaviourAction* moveToBonus = new BehaviourAction("Move to Coins",
		[&](float dt, BehaviourState state) -> BehaviourState {
			//Initialise Action
			if (state == Initialise) 
			{
				if (navGrid->FindPath(transform.GetPosition(), curTargetPos, path))
				{
					if (path.PopWaypoint(curWayPoint))
					{
						this->statename = "Collecting Coins";
						state = Ongoing;
					}
					else
					{
						state = Success;
					}
				}
				else
				{
					state = Failure;
				}
			}
			//Ongoing Action
			else if (state == Ongoing) {
				if (navGrid->isReached(transform.GetPosition(), curWayPoint))
				{
					if (path.PopWaypoint(curWayPoint)) 
					{
						DriveToPosition(curWayPoint);
					}
					else 
					{
						state = Failure;
					}
				}
				else 
				{
					DriveToPosition(curWayPoint);
				}
			}
			//Success Action
			else if (state == Success) {
				/*BehaviourRoot->Reset();*/
			}
			return state;
		}
	);

	BehaviourSelector* move = new BehaviourSelector("Move");
	move->AddChild(moveToBonus);
	move->AddChild(MoveToDest);

	BehaviourRoot->AddChild(move);

}
void EnemyBall::InitStateMachine()
{
	State* stateA = new State([&](float dt)-> void
		{
			path.Clear();
			startPos = Vector3((int)(transform.GetPosition().x / 10) * 10, 0, (int)(transform.GetPosition().z / 10) * 10);
			destinationPos = Vector3((int)(player->GetTransform().GetPosition().x / 10) * 10, 0, (int)(player->GetTransform().GetPosition().z / 10) * 10);
			path.PushWaypoint(destinationPos);
			navGrid->FindPath(startPos, destinationPos, path);
			DisplayPathfinding();
			path.PopWaypoint(curWayPoint);
			if (navGrid->isReached(transform.GetPosition(), curWayPoint))
			{
				path.PopWaypoint(curWayPoint);
				DriveToPosition(curWayPoint);
			}
			else
			{
				DriveToPosition(curWayPoint);
			}
			this->statename = "A* Chasing";
		}
	);

	State* stateB = new State([&](float dt)-> void
		{
			Vector3 movDir = (player->GetTransform().GetPosition() - transform.GetPosition()).Normalised();
			physicsObject->AddForce(movDir.Normalised() * (player->GetTransform().GetPosition() - transform.GetPosition()).Length());
			this->statename = "Raycast Chasing";
		}
	);

	machine->AddState(stateA);
	machine->AddState(stateB);

	machine->AddTransition(new StateTransition(stateA, stateB,
		[&]()->bool
		{
			return viewObject->GetName() == "Player";
		}
	));
	machine->AddTransition(new StateTransition(stateB, stateA,
		[&]()->bool
		{
			return viewObject->GetName() != "Player";
		}
	));
}
void EnemyBall::DriveToPosition(const Vector3 position)
{
	if (isFreezed)
	{
		return;
	}
	Vector3 moveDir = (position - transform.GetPosition()).Normalised();
	if (this->statename == "Collecting Coins")
	{
		physicsObject->AddForce(moveDir * (position - transform.GetPosition()).Length() * 2);
	}
	else
	{
		physicsObject->AddForce(moveDir * (position - transform.GetPosition()).Length() * 4);
	}
}

//Update issues
void EnemyBall::Update(float dt)
{
	//UpdatePath(dt);
	if (isFreezed)
	{
		freezeTime -= dt;
		if (freezeTime <= 0)
		{
			freezeTime = 5.0f;
			isFreezed = false;
		}
	}
	UpdateBehaviourTree(dt);

}
void EnemyBall::UpdateBehaviourTree(float dt)
{
	//update relativedistances
	Vector3 position = transform.GetPosition();
	delta = delta = player->GetTransform().GetPosition() - position;
	relativeDistance = delta.Length();

	//show curwaypoint direction
	Vector3 rayDir = Vector3((curWayPoint - position).x, 0, (curWayPoint - position).z).Normalised();
	Ray r(position, rayDir);
	Debug::DrawLine(r.GetPosition(), curWayPoint, Debug::BLUE, dt);
	Vector3 traceDir = (player->GetTransform().GetPosition() - position ).Normalised();
	Ray traceR(position + Vector3(0, 0, 1), traceDir);
	Debug::DrawLine(traceR.GetPosition(), player->GetTransform().GetPosition(), Debug::RED, dt);
	RayCollision collisionInfo;
	if (world)
	{
		world->Raycast(traceR, collisionInfo ,true);
		viewObject = (GameObject*)collisionInfo.node;
	}

	BehaviourRoot->Execute(dt);
}
void EnemyBall::DisplayPathfinding()
{
	pathNodes.clear();
	Vector3 pos;
	NavigationPath displayPath = path;
	while (displayPath.PopWaypoint(pos))
	{
		pathNodes.push_back(pos);
	}
	for (int i = 1; i < pathNodes.size(); ++i)
	{
		Vector3 a = pathNodes[i - 1];
		Vector3 b = pathNodes[i];

		Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
	}
}
void EnemyBall::UpdatePath(float dt)
{
	path.Clear();
	destinationPos = player->GetTransform().GetPosition();
	navGrid->FindPath(transform.GetPosition(), destinationPos, path);
	DisplayPathfinding();
}


