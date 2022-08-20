#pragma once
#include "..\CSC8503Common\GameObject.h"
#include "../CSC8503Common/GameWorld.h"
#include "../CSC8503Common/BehaviourNode.h"
#include "../CSC8503Common/BehaviourAction.h"
#include "../CSC8503Common/BehaviourNodeWithChildren.h"
#include "../CSC8503Common/BehaviourSelector.h"
#include "../CSC8503Common/BehaviourSequence.h"
#include "../CSC8503Common/PositionConstraint.h"
namespace NCL
{
	namespace CSC8503
	{
		class ConstraintObject : public GameObject
		{
		public:
			ConstraintObject(GameObject* player, string name = "");

			~ConstraintObject();

			virtual void Update(float dt);
		protected:
			BehaviourSequence* rootsequence;
			GameObject* player;
			GameWorld* gameworld;

			Vector3 delta;
			float relativeDistance;
		};


	}
}