#include "PhysicsWorld.h"
#include "../debug/Assert.h"

namespace engine {

	PhysicsWorld::PhysicsWorld() {
		this->physicsWorld = physicsCommon.createPhysicsWorld();
	}

	void PhysicsWorld::update(float deltaTime) {
		this->physicsWorld->update(deltaTime);
	}

	RigidBody PhysicsWorld::createRigidBody(const glm::vec3& position, const glm::vec3& rotation) {
		//create the physics engine RigidBody
		auto rotationAsQuaternion = reactphysics3d::Quaternion::fromEulerAngles({ rotation.x, rotation.y, rotation.z });
		reactphysics3d::Transform rigidBodyTransform({ position.x, position.y, position.z }, rotationAsQuaternion);
		reactphysics3d::RigidBody* rigidBody = this->physicsWorld->createRigidBody(rigidBodyTransform);

		// returns a RigidBBody wrapper
		return RigidBody{ &this->physicsCommon, rigidBody };
	}

}
