#include "CollisionBody.h"

#include "../debug/Assert.h"

namespace engine {

	CollisionBody::CollisionBody(reactphysics3d::PhysicsCommon* physicsCommon, reactphysics3d::CollisionBody* collisionBody) 
		: physicsCommon(physicsCommon), reactphysics3CollisionBody(collisionBody) {}

	void CollisionBody::addCollisionShape(const Transform& transform, CollisionShape collisionShape) {
		reactphysics3d::ConvexPolyhedronShape* shape = nullptr;

		switch (collisionShape) {
		case CollisionShape::Box: {
			shape = this->physicsCommon->createBoxShape({ transform.scale.x, transform.scale.y, transform.scale.z });
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
		this->reactphysics3CollisionBody->addCollider(shape, collisionShapeTransform);
	}

	void CollisionBody::setTransform(const Transform& transform) {
		auto rotationAsQuaternion = reactphysics3d::Quaternion::fromEulerAngles({ transform.rotation.x, transform.rotation.y, transform.rotation.z });
		reactphysics3d::Transform CollisionBodyTransform({ transform.position.x, transform.position.y, transform.position.z }, rotationAsQuaternion);
		this->reactphysics3CollisionBody->setTransform(CollisionBodyTransform);
	}
}