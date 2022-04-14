#pragma once

#include <glm/glm.hpp>
#include "RigidBody.h"
#include "CollisionBody.h"
#include <vector>
#pragma warning(push, 0)        
#include <reactphysics3d/reactphysics3d.h>
#pragma warning(pop)

#include "../debug/Log.h"
#include "../debug/Assert.h"

namespace engine {


	class CollisionCallback : public reactphysics3d::OverlapCallback {

	public:

		CollisionCallback(std::vector<CollisionBody*>& physicsWorldCollisionBodies, reactphysics3d::CollisionBody* targetCollisionBody)
			: physicsWorldCollisionBodies(physicsWorldCollisionBodies), targetCollisionBody(targetCollisionBody) {

		}

		virtual void onOverlap(reactphysics3d::OverlapCallback::CallbackData& callbackData) override;

		std::vector<CollisionBody*> getCollidingRigidBodies() {
			return collidingCollisionBodies;
		}

	private:
		std::vector<CollisionBody*> collidingCollisionBodies;
		std::vector<CollisionBody*>& physicsWorldCollisionBodies;

		reactphysics3d::CollisionBody* targetCollisionBody;

	};

	class PhysicsWorld {
	
	private:


		reactphysics3d::PhysicsCommon physicsCommon;
		reactphysics3d::PhysicsWorld* physicsWorld;
		std::vector<RigidBody*> rigidBodies;
		std::vector<CollisionBody*> collisionBodies;

	public:

		~PhysicsWorld();

		PhysicsWorld();

		void update(float deltaTime);

		RigidBody* createRigidBody(const glm::vec3& position, const glm::vec3& rotation);

		CollisionBody* createCollisionBody(const glm::vec3& position, const glm::vec3& rotation);

		void destroyRigidBody(RigidBody* rigidBody);

		void destroyCollisionBody(CollisionBody* rigidBody);

		std::vector<CollisionBody*> getCollidingBodies(CollisionBody* rigidBody);
	};

}