#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <glm/glm.hpp>
#include "../math/Transform.h"

namespace engine {
	
	enum class CollisionShape { Box, Sphere, Capsule };

	enum class RigidBodyType { Dynamic , Static, Kinematic };

	class RigidBody {

	private:
		reactphysics3d::PhysicsCommon* physicsCommon;
		reactphysics3d::RigidBody* rigidBodyPtr;
		reactphysics3d::Transform prevTransform;

	public:
		RigidBody();

		RigidBody(reactphysics3d::PhysicsCommon* physicsCommon, reactphysics3d::RigidBody* rigidBody);

		void addCollisionShape(const glm::vec3& scale, CollisionShape collisionShape);

		Transform getInterpolatedTranform(float timeInterpolationFactor);

		void setRigidBodyType(RigidBodyType rigidBodyType);

		void apllyForce(glm::vec3 force);

	};

}

