#pragma once

#include <glm/glm.hpp>
#include "../math/Transform.h"
#pragma warning(push, 0)        
#include <reactphysics3d/reactphysics3d.h>
#pragma warning(pop)

#include "CollisionShape.h"

namespace engine {
	
	enum class RigidBodyType { Dynamic , Static, Kinematic };

	class RigidBody {

		friend class PhysicsWorld;

	private:

		RigidBodyType rigidBodyType;

		reactphysics3d::PhysicsCommon* physicsCommon;
		reactphysics3d::RigidBody* rigidBodyPtr;
		reactphysics3d::Transform prevTransform;

		RigidBody();

		RigidBody(reactphysics3d::PhysicsCommon* physicsCommon, reactphysics3d::RigidBody* rigidBody);
	
	public:

		void addCollisionShape(const glm::vec3& scale, CollisionShape collisionShape);

		Transform getInterpolatedTranform(float timeInterpolationFactor);

		void setRigidBodyType(RigidBodyType rigidBodyType);

		void apllyForce(const glm::vec3& force);
		
		void setTransform(const Transform& transform);

		RigidBodyType getRigidBodyType() { return this->rigidBodyType;  }

	};

}

