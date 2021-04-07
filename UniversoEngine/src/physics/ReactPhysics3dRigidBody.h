#pragma once

#include "RigidBody.h"
#include <reactphysics3d/reactphysics3d.h>

namespace engine {

	class ReactPhysics3dRigidBody : public RigidBody {

	private:

		reactphysics3d::PhysicsCommon* physicsCommon;
		reactphysics3d::RigidBody* rigidBodyPtr;
		reactphysics3d::Transform prevTransform;

	public:

		ReactPhysics3dRigidBody();

		ReactPhysics3dRigidBody(reactphysics3d::PhysicsCommon* physicsCommon, reactphysics3d::RigidBody* rigidBody);

		void addCollisionShape(const glm::vec3& scale, CollisionShape collisionShape);

		Transform getInterpolatedTranform(float timeInterpolationFactor);

		void setRigidBodyType(RigidBodyType rigidBodyType);

		void apllyForce(glm::vec3 force);
	};
} 
