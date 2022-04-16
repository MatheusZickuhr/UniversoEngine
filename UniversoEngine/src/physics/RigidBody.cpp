#include "RigidBody.h"
#include <glm/gtx/quaternion.hpp>
#include "../debug/Assert.h"


namespace engine {

	RigidBody::RigidBody(reactphysics3d::PhysicsCommon* physicsCommon, reactphysics3d::RigidBody* rigidBody)
		: CollisionBody(physicsCommon, rigidBody), 
		reactphysics3dRigidBody(rigidBody),
		prevTransform(rigidBody->getTransform()),
		rigidBodyType(RigidBodyType::Dynamic) { }

	Transform RigidBody::getInterpolatedTranform(float timeInterpolationFactor) {
		auto& currentTransform = reactphysics3dRigidBody->getTransform();
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
			reactphysics3dRigidBody->setType(reactphysics3d::BodyType::DYNAMIC);
			break;
		}
		case RigidBodyType::Static: {
			reactphysics3dRigidBody->setType(reactphysics3d::BodyType::STATIC);
			break;
		}
		case RigidBodyType::Kinematic: {
			reactphysics3dRigidBody->setType(reactphysics3d::BodyType::KINEMATIC);
			break;
		}
		}
	}

	void RigidBody::apllyForce(const glm::vec3& force) {
		reactphysics3dRigidBody->applyLocalForceAtCenterOfMass(reactphysics3d::Vector3(force.x, force.y, force.z));
	}

}