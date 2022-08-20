#include "InvStateObject.h"
#include "../CSC8503Common//StateTransition.h"
#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/State.h"

using namespace NCL;
using namespace CSC8503;

InvStateGameObject::InvStateGameObject(string name)
{
	counter = 0.0f;
	stateMachine = new StateMachine();
	this->name = name;

	State* stateA = new State([&](float dt)-> void
		{
			this->MoveLeft(dt);
			this->statename = "MOVE LEFT";
		}
	);
	State* stateB = new State([&](float dt)->void
		{
			this->MoveRight(dt);
			this->statename = "MOVE RIGHT";
		}
	);

	stateMachine->AddState(stateB);
	stateMachine->AddState(stateA);

	stateMachine->AddTransition(new StateTransition(stateB, stateA,
		[&]()->bool
		{
			return this->counter > 1.0f;
		}
	));
	stateMachine->AddTransition(new StateTransition(stateA, stateB,
		[&]()->bool
		{
			return this->counter < 0.0f;
		}
	));
}

InvStateGameObject::~InvStateGameObject()
{
	delete stateMachine;
}

void InvStateGameObject::Update(float dt)
{
	stateMachine->Update(dt);
}

void InvStateGameObject::MoveLeft(float dt)
{
	GetPhysicsObject()->SetLinearVelocity(Vector3(0, 0, -20));
	counter -= dt;
}

void InvStateGameObject::MoveRight(float dt)
{
	GetPhysicsObject()->SetLinearVelocity(Vector3(0, 0, 20));
	counter += dt;
}
