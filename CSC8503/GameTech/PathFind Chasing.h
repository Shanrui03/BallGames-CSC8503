#pragma once
#include "../CSC8503Common/PushdownMachine.h"
#include "../CSC8503Common/PushdownState.h"
#include "../../Common/Window.h"
#include "EnemyBall.h"
using namespace NCL;
using namespace CSC8503;

class PathFind :public PushdownState
{
public:
	PathFind(EnemyBall* enemyBall)
	{
		this->enemyBall = enemyBall;
	}

	PushdownResult PushdownUpdate(float dt, PushdownState** newState) override
	{
		return PushdownResult::NoChange;
	};

	void OnAwake() override
	{

	}
protected:
	EnemyBall* enemyBall;
};