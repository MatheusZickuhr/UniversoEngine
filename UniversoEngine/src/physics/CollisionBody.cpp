#include "CollisionBody.h"

#include "../debug/Assert.h"

namespace engine {

	CollisionBody::CollisionBody(reactphysics3d::PhysicsCommon* physicsCommon, reactphysics3d::CollisionBody* collisionBody) 
		: physicsCommon(physicsCommon), collisionBodyPtr(collisionBody) {}

	void CollisionBody::addCollisionShape(const glm::vec3& scale, CollisionShape collisionShape) {
		reactphysics3d::ConvexPolyhedronShape* shape = nullptr;

		switch (collisionShape) {
		case CollisionShape::Box: {
			shape = this->physicsCommon->createBoxShape({ scale.x, scale.y, scale.z });
			break;
		}
		case CollisionShape::Capsule: {

			break;
		}
		case CollisionShape::Sphere: {

			break;
		}
		}

		ASSERT(shape != nullptr, "You must choose a valid/implemented collision shape");

		reactphysics3d::Transform collisionShapeTransform;
		this->collisionBodyPtr->addCollider(shape, collisionShapeTransform);
	}

	void CollisionBody::setTransform(const Transform& transform) {
		auto rotationAsQuaternion = reactphysics3d::Quaternion::fromEulerAngles({ transform.rotation.x, transform.rotation.y, transform.rotation.z });
		reactphysics3d::Transform CollisionBodyTransform({ transform.position.x, transform.position.y, transform.position.z }, rotationAsQuaternion);
		this->collisionBodyPtr->setTransform(CollisionBodyTransform);
	}
}