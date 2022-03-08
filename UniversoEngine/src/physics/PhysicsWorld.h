#pragma once

#include <glm/glm.hpp>
#include "RigidBody.h"
#include <vector>
#pragma warning(push, 0)        
#include <reactphysics3d/reactphysics3d.h>
#pragma warning(pop)

namespace engine {

	class PhysicsWorld {
	
	private:

		reactphysics3d::PhysicsCommon physicsCommon;
		reactphysics3d::PhysicsWorld* physicsWorld;
		std::vector<RigidBody*> rigidBodies;

	public:

		~PhysicsWorld();

		PhysicsWorld();

		void update(float deltaTime);

		RigidBody* createRigidBody(const glm::vec3& position, const glm::vec3& rotation);

		void destroyRigidBody(RigidBody* rigidBody);
	};

}