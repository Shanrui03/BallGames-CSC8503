#pragma once
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../CSC8503Common/PositionConstraint.h"
#include "../CSC8503Common/NavigationGrid.h"
#include "../../Common/Quaternion.h"
#include "InvStateObject.h"
#include "StateGameObject.h"
#include "PointCoins.h"
#include "ConstraintObject.h"
#include "EnemyBall.h"


namespace NCL {
	namespace CSC8503 {
		class TutorialGame		{
		public:
			TutorialGame();
			~TutorialGame();

			//Present the scene
			virtual void UpdateGame(float dt);
			void UpdateNormalStateObject(float dt);
			void InitWorld(bool isDebug = false);
			void InitHardGameWorld(bool isDebug = false);
			void UpdateKeys(bool isHard);

			//Getters
			GameWorld* GetGameWorld();
			PhysicsSystem* GetPhysics();
			GameObject* selectionObject = nullptr;

			vector<Vector3> testNodes;
		protected:
			void InitialiseAssets();
			void InitCamera(bool isHard);
			void InitGameExamples();
			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims);
			void BridgeConstraintTest();

			void InitDefaultFloor();
			//Mode 1
			void InitStateObjects();
			void InitSpecialItems();
			void InitCapsuleAndOBB();
			//Mode 2
			void InitMap();
			void InitEnemyAndPlayer();
			void InitSpecialItems2();

	
			bool SelectObject();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();

			//pathfindg test
			void TestPathfinding();
			void DisplayPathfinding();

			GameObject* AddFloorToWorld(const Vector3& position,Vector3 dimensions);
			GameObject* AddSlopingFloorToWorld(const Vector3& position, const Quaternion& orientation);
			GameObject* AddSphereToWorld(const Vector3& position, float radius, string name = "",float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, string name = "",float inverseMass = 10.0f,float friction = 0.8f);
			GameObject* AddOBBToWorld(const Vector3& position, const Quaternion& orientation,Vector3 dimensions, string name = "", float inverseMass = 10.0f);
			GameObject* AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius, string name = "", float inverseMass = 10.0f, float elasticity = 0.8f);
			GameObject* AddPlayerToWorld(const Vector3& position);
			StateGameObject* AddEnemyToWorld(const Vector3& position);
			InvStateGameObject* AddEnemy02ToWorld(const Vector3& position);
			GameObject* AddBonusToWorld(const Vector3& position,string name = "");
			StateGameObject* AddStateObjectToWorld(const Vector3& position, float halfHeight, float radius, string name, float inverseMass);
			PointCoins* AddPointCoinsToWorld(const Vector3& position);
			ConstraintObject* AddConstraintObjectToWorld(const Vector3& position, float halfHeight,float radius,GameObject* player, string name = "",float inverseMass = 10.0f);
			EnemyBall* AddEnemyBallToWorld(const Vector3& position);
			

			GameTechRenderer*	renderer;
			PhysicsSystem*		physics;
			GameWorld*			world;

			StateGameObject* enemy1;
			InvStateGameObject* enemy2;

			PointCoins* pointcoin;
			PointCoins* pointcoin2;
			PointCoins* pointcoin3;
			PointCoins* pointcoin4;
			PointCoins* pointcoin5;
			PointCoins* pointcoin6;

			PositionConstraint* attachmentConstraint;
			PositionConstraint* attachmentConstraint2;
			ConstraintObject* constraintSphere;
			ConstraintObject* constraintSphere2;
			GameObject* player;
			GameObject* player2;
			EnemyBall* enemyball;

			bool useGravity;
			bool inSelectionMode;

			float		forceMagnitude;


			OGLMesh*	capsuleMesh = nullptr;
			OGLMesh*	cubeMesh	= nullptr;
			OGLMesh*	sphereMesh	= nullptr;
			OGLTexture* basicTex	= nullptr;
			OGLTexture* dogeTex = nullptr;
			OGLTexture* catTex = nullptr;
			OGLTexture* iceTex = nullptr;
			OGLShader*	basicShader = nullptr;

			//Coursework Meshes
			OGLMesh*	charMeshA	= nullptr;
			OGLMesh*	charMeshB	= nullptr;
			OGLMesh*	enemyMesh	= nullptr;
			OGLMesh*	bonusMesh	= nullptr;

			//Coursework Additional functionality	
			GameObject* lockedObject	= nullptr;
			Vector3 lockedOffset		= Vector3(0, 100, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}

			//Navigation map
			NavigationGrid* gridMap;
			NavigationPath outPath;

		};
	}
}

