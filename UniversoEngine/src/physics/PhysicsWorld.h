#pragma once

#include <glm/glm.hpp>
#include "RigidBody.h"

namespace engine {

	class PhysicsWorld {
	
	public:

		virtual void update(float deltaTime) = 0;

		virtual RigidBody* createRigidBody(const glm::vec3& position, const glm::vec3& rotation) = 0;

	};

}