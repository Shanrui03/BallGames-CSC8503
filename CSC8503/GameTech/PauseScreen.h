#pragma once
#include "../CSC8503Common/PushdownMachine.h"
#include "../CSC8503Common/PushdownState.h"
#include "../../Common/Window.h"
using namespace NCL;
using namespace CSC8503;

class PauseScreen :public PushdownState
{
public:
	PauseScreen(TutorialGame* gameworld)
	{
		this->gameworld = gameworld;
	}
	PushdownResult PushdownUpdate(float dt, PushdownState** newState) override
	{
   		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::U))
		{
			return PushdownResult::Pop;
		}
		Debug::Print("Pause!", Vector2(5, 50));
		Debug::Print("Press U To Restart!", Vector2(5, 60));
		gameworld->UpdateGame(0.0f);
		return PushdownResult::NoChange;
	}

	void OnAwake() override
	{

	}
protected:
	TutorialGame* gameworld;
};