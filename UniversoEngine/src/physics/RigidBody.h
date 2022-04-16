#pragma once

#include <glm/glm.hpp>
#include "../math/Transform.h"
#pragma warning(push, 0)        
#include <reactphysics3d/reactphysics3d.h>
#pragma warning(pop)

#include "CollisionShape.h"
#include "CollisionBody.h"

namespace engine {
	
	enum class RigidBodyType { Dynamic , Static, Kinematic };

	class RigidBody : public CollisionBody {

		friend class PhysicsWorld;
	
	public:

		RigidBody(reactphysics3d::PhysicsCommon* physicsCommon, reactphysics3d::RigidBody* rigidBody);

		Transform getInterpolatedTranform(float timeInterpolationFactor);

		void setRigidBodyType(RigidBodyType rigidBodyType);

		void apllyForce(const glm::vec3& force);

		RigidBodyType getRigidBodyType() { return this->rigidBodyType; }


	private:

		RigidBodyType rigidBodyType;
		reactphysics3d::Transform prevTransform;
		reactphysics3d::RigidBody* reactphysics3dRigidBody;
	};

}

