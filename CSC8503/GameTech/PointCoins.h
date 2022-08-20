#pragma once
#include "..\CSC8503Common\GameObject.h"
namespace NCL
{
	namespace CSC8503
	{
		class StateMachine;
		class PointCoins : public GameObject
		{
		public:
			PointCoins(string name = "");
			~PointCoins();

			virtual void Update(float dt);


		protected:
			float counter;
			StateMachine* stateMachine;

			void MoveUp(float dt);
			void MoveDown(float dt);
		};
	}
}