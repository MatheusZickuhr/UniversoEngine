#pragma once
#include "RigidBody.h"
#include "../math/Transform.h"
#include <reactphysics3d/reactphysics3d.h>

namespace engine {

	class PhysicsWorld {

	private:

		reactphysics3d::PhysicsCommon physicsCommon;
		reactphysics3d::PhysicsWorld* physicsWorld;

	public:

		PhysicsWorld();

		void update(float deltaTime);

		RigidBody createRigidBody(const glm::vec3& position, const glm::vec3& rotation);

	};

}