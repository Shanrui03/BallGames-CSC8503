#include "StateGameObject.h"
#include "../CSC8503Common//StateTransition.h"
#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/State.h"

using namespace NCL;
using namespace CSC8503;

StateGameObject::StateGameObject(string name)
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

	stateMachine->AddState(stateA);
	stateMachine->AddState(stateB);

	stateMachine->AddTransition(new StateTransition(stateA, stateB,
		[&]()->bool
		{
			return this->counter > 1.0f;
		}
	));
	stateMachine->AddTransition(new StateTransition(stateB, stateA,
		[&]()->bool
		{
			return this->counter < 0.0f;
		}
	));
}

StateGameObject::~StateGameObject()
{
	delete stateMachine;
}

void StateGameObject::Update(float dt)
{
	stateMachine->Update(dt);
}

void StateGameObject::MoveLeft(float dt)
{
	GetPhysicsObject()->SetLinearVelocity(Vector3(0, 0, -20));
	counter += dt;
}

void StateGameObject::MoveRight(float dt)
{
	GetPhysicsObject()->SetLinearVelocity(Vector3(0, 0, 20));
	counter -= dt;
}
