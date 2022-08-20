#pragma once
#include "../CSC8503Common/PushdownMachine.h"
#include "../CSC8503Common/PushdownState.h"
#include "../../Common/Window.h"
#include "PauseScreen.h"
#include "TutorialGame.h"
#include <ctime>
using namespace NCL;
using namespace CSC8503;

class HardGameScreen :public PushdownState
{
public:
	HardGameScreen(TutorialGame* gameworld)
	{
		this->gameworld = gameworld;
		gameworld->InitHardGameWorld();
		start = clock();
	}
	PushdownResult PushdownUpdate(float dt, PushdownState** newState) override
	{
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::P))
		{
			*newState = new PauseScreen(gameworld);
			return PushdownResult::Push;
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::F1))
		{
			std::cout << "Returning to main menu!\n";
			gameworld->GetGameWorld()->Clear();
			gameworld->GetPhysics()->Clear();
			return PushdownResult::Pop;
		}
		gameworld->UpdateKeys(true);
		if (!gameworld->GetPhysics()->isGameDead)
		{
			gameworld->UpdateGame(dt);
		}
		else
		{
			gameworld->UpdateGame(0.0f);
		}
		if (!gameworld->GetPhysics()->isGameEnd && !gameworld->GetPhysics()->isGameDead)
		{
			Debug::Print("Game is developing!", Vector2(5, 50));
			Debug::Print("Press P To Pause or Press F1 To Return!", Vector2(5, 60));
			Debug::Print("Press F2 To Restart!", Vector2(5, 70));
		}
		else if (gameworld->GetPhysics()->isGameEnd && !gameworld->GetPhysics()->isGameDead)
		{
			start = gameworld->GetPhysics()->StartTime;
			end = gameworld->GetPhysics()->EndTime;
			Debug::Print("Complete Game!Congratulations!", Vector2(30, 45));
			Debug::Print("Total Score : " + std::to_string(gameworld->GetPhysics()->score), Vector2(30, 50));
			Debug::Print("Time Spent : " + std::to_string((end - start) / 1000) + " s", Vector2(30, 55));
			Debug::Print("Press F2 To Restart or Press F1 To Return!", Vector2(20, 60));
		}
		else if (!gameworld->GetPhysics()->isGameEnd && gameworld->GetPhysics()->isGameDead)
		{
			start = gameworld->GetPhysics()->StartTime;
			end = gameworld->GetPhysics()->EndTime;
			Debug::Print("YOU DIED", Vector2(30, 50));
			Debug::Print("Press F2 To Restart or Press F1 To Return!", Vector2(20, 60));
		}
		return PushdownResult::NoChange;
	};

	void OnAwake() override
	{

	}

protected:
	clock_t start, end;
	TutorialGame* gameworld;
};