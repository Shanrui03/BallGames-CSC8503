#include "PointCoins.h"
#include "../CSC8503Common//StateTransition.h"
#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/State.h"

using namespace NCL;
using namespace CSC8503;

PointCoins::PointCoins(string name)
{
	counter = 0.0f;
	stateMachine = new StateMachine();
	this->name = name;

	State* stateA = new State([&](float dt)-> void
		{
			this->MoveUp(dt);
			this->statename = "MOVE UP";
		}
	);
	State* stateB = new State([&](float dt)->void
		{
			this->MoveDown(dt);
			this->statename = "MOVE DOWN";
		}
	);

	stateMachine->AddState(stateA);
	stateMachine->AddState(stateB);

	stateMachine->AddTransition(new StateTransition(stateA, stateB,
		[&]()->bool
		{
			return this->counter > 0.5f;
		}
	));
	stateMachine->AddTransition(new StateTransition(stateB, stateA,
		[&]()->bool
		{
			return this->counter < 0.0f;
		}
	));
}

PointCoins::~PointCoins()
{
	delete stateMachine;
}

void PointCoins::Update(float dt)
{
	stateMachine->Update(dt);
}

void PointCoins::MoveUp(float dt)
{
	GetPhysicsObject()->AddForce({ 0,20,0 });
	counter += dt;
}

void PointCoins::MoveDown(float dt)
{
	GetPhysicsObject()->AddForce({ 0,-20,0 });
	counter -= dt;
}
