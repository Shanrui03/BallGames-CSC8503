#pragma once
#include "..\CSC8503Common\GameObject.h"
#include "../CSC8503Common/GameWorld.h"
#include "../CSC8503Common/BehaviourNode.h"
#include "../CSC8503Common/BehaviourAction.h"
#include "../CSC8503Common/BehaviourNodeWithChildren.h"
#include "../CSC8503Common/BehaviourSelector.h"
#include "../CSC8503Common/BehaviourSequence.h"
#include "../CSC8503Common/NavigationMap.h"
#include "../CSC8503Common/NavigationPath.h"
#include "../CSC8503Common/NavigationGrid.h"
#include "../CSC8503Common/PositionConstraint.h"
#include "../CSC8503Common//StateTransition.h"
#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/State.h"
#include "ConstraintObject.h"
#include "PointCoins.h"

namespace NCL {
	namespace CSC8503 {
		class EnemyBall : public GameObject
		{
		public:
			EnemyBall(NavigationGrid* gridMap, GameObject* player, ConstraintObject* constraintSphere, PointCoins* coin, GameWorld* world = nullptr, string name = "")
			{
				navGrid = gridMap;
				machine = new StateMachine();
				this->player = player;
				this->viewObject = nullptr;
				this->name = name;
				this->constraintSphere = constraintSphere;
				this->curTarget = coin;
				this->world = world;
				this->curTargetPos = curTarget->GetTransform().GetPosition();

				curWayPoint = player->GetTransform().GetPosition();
				destinationPos = player->GetTransform().GetPosition();
				startdelta = Vector3(70, 0, 0) - Vector3(30, 0, 90);
				startrelativeDistance = startdelta.Length();

				InitStateMachine();
				InitBehaviourTree();
			}

			~EnemyBall()
			{
				delete machine;
			}

			void Update(float dt);
			void UpdatePath(float dt);
			void Respawn();

			vector<Vector3> pathNodes;
		protected:
			Vector3 delta;
			Vector3 startdelta;
			float startrelativeDistance;
			float relativeDistance;
			float freezeTime = 5.0f;


			/*Behaviour Tree*/
			BehaviourNodeWithChildren* BehaviourRoot;
			void InitBehaviourTree();
			void InitStateMachine();
			void UpdateBehaviourTree(float dt);
			void DriveToPosition(const Vector3 position);
			void DisplayPathfinding();

			GameObject* player;
			GameObject* viewObject;
			ConstraintObject* constraintSphere;
			PointCoins* curTarget;
			GameWorld* world;

			/*Navigation*/
			NavigationGrid* navGrid;
			NavigationPath path;
			Vector3 curWayPoint;
			Vector3 startPos;
			Vector3 destinationPos;
			Vector3 curTargetPos;

			/*Statemachine*/
			StateMachine* machine;
		};

	}
}