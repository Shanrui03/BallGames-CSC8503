#pragma once
#include "..\CSC8503Common\GameObject.h"
namespace NCL
{
	namespace CSC8503
	{
		class StateMachine;
		class InvStateGameObject : public GameObject
		{
		public:
			InvStateGameObject(string name = "");
			~InvStateGameObject();

			virtual void Update(float dt);

		protected:
			void MoveLeft(float dt);
			void MoveRight(float dt);

			StateMachine* stateMachine;
			float counter;
		};
	}
}