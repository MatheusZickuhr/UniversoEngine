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

private:

	reactphysics3d::PhysicsCommon* physicsCommon;
	reactphysics3d::CollisionBody* collisionBodyPtr;
	reactphysics3d::Transform prevTransform;

	CollisionBody();

	CollisionBody(reactphysics3d::PhysicsCommon* physicsCommon, reactphysics3d::CollisionBody* collisionBody);

public:

	void addCollisionShape(const glm::vec3& scale, CollisionShape collisionShape);

	void setTransform(const Transform& transform);
};

}
