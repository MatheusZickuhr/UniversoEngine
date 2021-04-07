#pragma once

#include "PhysicsWorld.h"
#include "ReactPhysics3dRigidBody.h"

namespace engine {

	class ReactPhysics3dPhysicsWorld : public PhysicsWorld {

	private:

		reactphysics3d::PhysicsCommon physicsCommon;
		reactphysics3d::PhysicsWorld* physicsWorld;

	public:

		ReactPhysics3dPhysicsWorld();

		void update(float deltaTime);

		RigidBody* createRigidBody(const glm::vec3& position, const glm::vec3& rotation);
	};
}