#include "RigidBody.h"

namespace engine {

	RigidBody::RigidBody() {}

	RigidBody::RigidBody(reactphysics3d::RigidBody* rigidBody)
		: rigidBodyPtr(rigidBody), prevTransform(rigidBody->getTransform()) {
	}

	void RigidBody::apllyForce(glm::vec3 force) {
		rigidBodyPtr->applyForceToCenterOfMass(reactphysics3d::Vector3(force.x, force.y, force.z));
	}

}
