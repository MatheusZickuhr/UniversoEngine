#pragma once

#include <glm/glm.hpp>
#include "../math/Transform.h"

namespace engine {
	
	enum class CollisionShape { Box, Sphere, Capsule };

	enum class RigidBodyType { Dynamic , Static, Kinematic };

	class RigidBody {

	public:
		virtual void addCollisionShape(const glm::vec3& scale, CollisionShape collisionShape) = 0;

		virtual Transform getInterpolatedTranform(float timeInterpolationFactor) = 0;

		virtual void setRigidBodyType(RigidBodyType rigidBodyType) = 0;

		virtual void apllyForce(glm::vec3 force) = 0;

	};

}

