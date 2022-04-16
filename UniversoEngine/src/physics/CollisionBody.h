#pragma once

#include <glm/glm.hpp>
#include "../math/Transform.h"
#pragma warning(push, 0)        
#include <reactphysics3d/reactphysics3d.h>
#pragma warning(pop)

#include "CollisionShape.h"

namespace engine {

	class CollisionBody {
	
		friend class PhysicsWorld;
		friend class CollisionCallback;


	public:

		void addCollisionShape(const Transform& transform, CollisionShape collisionShape);

		void setTransform(const Transform& transform);

		CollisionBody(reactphysics3d::PhysicsCommon* physicsCommon, reactphysics3d::CollisionBody* collisionBody);
	
	protected:

		reactphysics3d::PhysicsCommon* physicsCommon;

	private:

		reactphysics3d::CollisionBody* reactphysics3CollisionBody;
	};

}
