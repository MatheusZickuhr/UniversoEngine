#pragma once

#include <glm/glm.hpp>
#include "../math/Transform.h"
#include <reactphysics3d/reactphysics3d.h>

namespace engine {
	
	enum class CollisionShape { Box, Sphere, Capsule };

	enum class RigidBodyType { Dynamic , Static, Kinematic };

	class RigidBody {

		friend class PhysicsWorld;

	private:

		reactphysics3d::PhysicsCommon* physicsCommon;
		reactphysics3d::RigidBody* rigidBodyPtr;
		reactphysics3d::Transform prevTransform;

		RigidBody();

		RigidBody(reactphysics3d::PhysicsCommon* physicsCommon, reactphysics3d::RigidBody* rigidBody);
	
	public:

		void addCollisionShape(const glm::vec3& scale, CollisionShape collisionShape);

		Transform getInterpolatedTranform(float timeInterpolationFactor);

		void setRigidBodyType(RigidBodyType rigidBodyType);

		void apllyForce(glm::vec3 force);

	};

}

