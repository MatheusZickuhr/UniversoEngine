#include "RigidBody.h"
#include <glm/gtx/quaternion.hpp>
#include "../debug/Assert.h"


namespace engine {
	RigidBody::RigidBody() {}

	RigidBody::RigidBody(reactphysics3d::PhysicsCommon* physicsCommon, reactphysics3d::RigidBody* rigidBody)
		: physicsCommon(physicsCommon), rigidBodyPtr(rigidBody), prevTransform(rigidBody->getTransform()) {
	}

	void RigidBody::addCollisionShape(const glm::vec3& scale, CollisionShape collisionShape) {
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
		this->rigidBodyPtr->addCollider(shape, collisionShapeTransform);
	}

	Transform RigidBody::getInterpolatedTranform(float timeInterpolationFactor) {
		auto& currentTransform = this->rigidBodyPtr->getTransform();
		auto interpolatedTransform = reactphysics3d::Transform::interpolateTransforms(
			this->prevTransform, currentTransform, timeInterpolationFactor);
		this->prevTransform = currentTransform;

		Transform transform;

		auto& position = interpolatedTransform.getPosition();
		transform.position = { position.x , position.y , position.z };

		auto& orientation = interpolatedTransform.getOrientation();
		transform.rotation = glm::eulerAngles(
			glm::quat(orientation.w, orientation.x, orientation.y, orientation.z)
		);

		return transform;
	}

	void RigidBody::setRigidBodyType(RigidBodyType rigidBodyType) {
		this->rigidBodyType = rigidBodyType;
		
		switch (rigidBodyType) {
		case RigidBodyType::Dynamic: {
			this->rigidBodyPtr->setType(reactphysics3d::BodyType::DYNAMIC);
			break;
		}
		case RigidBodyType::Static: {
			this->rigidBodyPtr->setType(reactphysics3d::BodyType::STATIC);
			break;
		}
		case RigidBodyType::Kinematic: {
			this->rigidBodyPtr->setType(reactphysics3d::BodyType::KINEMATIC);
			break;
		}
		}
	}

	void RigidBody::apllyForce(const glm::vec3& force) {
		rigidBodyPtr->applyLocalForceAtCenterOfMass(reactphysics3d::Vector3(force.x, force.y, force.z));
	}

	void RigidBody::setTransform(const Transform& transform) {
		auto rotationAsQuaternion = reactphysics3d::Quaternion::fromEulerAngles({ transform.rotation.x, transform.rotation.y, transform.rotation.z });
		reactphysics3d::Transform rigidBodyTransform({ transform.position.x, transform.position.y, transform.position.z }, rotationAsQuaternion);
		this->rigidBodyPtr->setTransform(rigidBodyTransform);
	}
}