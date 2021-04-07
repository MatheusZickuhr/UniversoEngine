#pragma once

#include "PhysicsWorld.h"
#include <vector>
#include <reactphysics3d/reactphysics3d.h>

namespace engine {

	class ReactPhysics3dPhysicsWorld : public PhysicsWorld {

	private:

		reactphysics3d::PhysicsCommon physicsCommon;
		reactphysics3d::PhysicsWorld* physicsWorld;
		std::vector<RigidBody*> rigidBodies;

	public:

		~ReactPhysics3dPhysicsWorld();

		ReactPhysics3dPhysicsWorld();

		void update(float deltaTime);

		RigidBody* createRigidBody(const glm::vec3& position, const glm::vec3& rotation);
	};
}