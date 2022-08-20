#pragma once
#include "../CSC8503Common/PushdownMachine.h"
#include "../CSC8503Common/PushdownState.h"
#include "../../Common/Window.h"
#include "GameScreen.h"
#include "TutorialGame.h"
#include "HardGameScreen.h"
using namespace NCL;
using namespace CSC8503;

class IntroScreen :public PushdownState
{
public:
	IntroScreen(TutorialGame* gameworld)
	{
		this->gameworld = gameworld;
	}

	PushdownResult PushdownUpdate(float dt, PushdownState** newState) override
	{
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Z))
		{
			*newState = new GameScreen(gameworld);
			return PushdownResult::Push;
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::X))
		{
			*newState = new HardGameScreen(gameworld);
			return PushdownResult::Push;
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE))
		{
			return PushdownResult::Pop;
		}
		gameworld->UpdateGame(0.0f);
		Debug::Print("Welcome to Game!", Vector2(5, 50));
		Debug::Print("Press Z To Begin or Press Escape to quit!", Vector2(5, 60));
		Debug::Print("Press X To Racing Mode!", Vector2(5, 70));
		return PushdownResult::NoChange;
	};

	void OnAwake() override
	{

	}
protected:
	TutorialGame* gameworld;
};