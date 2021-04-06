#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <glm/glm.hpp>

namespace engine {
	
	enum class CollisionShape { Box, Sphere, Capsule };

	enum class RigidBodyType { Dynamic , Static, Kinematic };

	class RigidBody {
		friend class Engine;

	private:
		reactphysics3d::RigidBody* rigidBodyPtr;
		reactphysics3d::Transform prevTransform;

	public:
		RigidBody();

		RigidBody(reactphysics3d::RigidBody* rigidBody);

		void apllyForce(glm::vec3 force);

	};

}

