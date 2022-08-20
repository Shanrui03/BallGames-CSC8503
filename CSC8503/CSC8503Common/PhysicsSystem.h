#pragma once
#include "../CSC8503Common/GameWorld.h"
#include <set>
#include <ctime>

namespace NCL {
	namespace CSC8503 {
		class PhysicsSystem	{
		public:
			PhysicsSystem(GameWorld& g);
			~PhysicsSystem();

			void Clear();

			void Update(float dt);

			void UseGravity(bool state) {
				applyGravity = state;
			}

			void SetGlobalDamping(float d) {
				globalDamping = d;
			}

			void SetGravity(const Vector3& g);

			std::set<CollisionDetection::CollisionInfo> GetallCollisions()
			{
				return allCollisions;
			}

			int score = 200;
			float elapsedTime = 1.0f;
			bool isGameEnd = false;
			bool isGameDead = false;
			clock_t EndTime,StartTime;
		protected:
			void BasicCollisionDetection();
			void BroadPhase();
			void NarrowPhase();

			void ClearForces();

			void IntegrateAccel(float dt);
			void IntegrateVelocity(float dt);

			void UpdateConstraints(float dt);

			void UpdateCollisionList();
			void UpdateObjectAABBs();

			void ImpulseResolveCollision(GameObject& a , GameObject&b, CollisionDetection::ContactPoint& p);
			void BeginShootingImpulseResolveCollision(GameObject& shooter, GameObject& b, CollisionDetection::ContactPoint& p);
			void BeginShooting2ImpulseResolveCollision(GameObject& shooter2, GameObject& b, CollisionDetection::ContactPoint& p);
			void PointCoinsImpulseResolveCollision(GameObject& coin, GameObject& b, CollisionDetection::ContactPoint& p);
			void DestinationImpulseResolveCollision(GameObject& dest, GameObject& b, CollisionDetection::ContactPoint& p);
			void InvSteeringImpulseResolveCollision(GameObject& invsteering, GameObject& b, CollisionDetection::ContactPoint& p);
			void IceImpulseResolveCollision(GameObject& ice, GameObject& b, CollisionDetection::ContactPoint& p);
			void EnemyImpulseResolveCollision(GameObject& enemy, GameObject& b, CollisionDetection::ContactPoint& p);
			void SpringResolveCollision(GameObject& a, GameObject& b, CollisionDetection::ContactPoint& p);

			GameWorld& gameWorld;

			bool	applyGravity;
			Vector3 gravity;
			float	dTOffset;
			float	globalDamping;

			std::set<CollisionDetection::CollisionInfo> allCollisions;
			std::set<CollisionDetection::CollisionInfo> broadphaseCollisions;

			bool useBroadPhase		= true;
			int numCollisionFrames	= 5;
		};
	}
}

