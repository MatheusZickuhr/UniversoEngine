#include "PhysicsWorld.h"

namespace engine {
	
	PhysicsWorld::~PhysicsWorld() {
		for (auto rigidBody : this->rigidBodies) {
			delete rigidBody;
		}
		this->rigidBodies.clear();
		this->physicsCommon.destroyPhysicsWorld(this->physicsWorld);
	}

	PhysicsWorld::PhysicsWorld() {
		this->physicsWorld = physicsCommon.createPhysicsWorld();
	}

	void PhysicsWorld::update(float deltaTime) {
		this->physicsWorld->update(deltaTime);
	}

	RigidBody* PhysicsWorld::createRigidBody(const glm::vec3& position, const glm::vec3& rotation) {
		//create the physics engine RigidBody
		auto rotationAsQuaternion = reactphysics3d::Quaternion::fromEulerAngles({ rotation.x, rotation.y, rotation.z });
		reactphysics3d::Transform rigidBodyTransform({ position.x, position.y, position.z }, rotationAsQuaternion);
		reactphysics3d::RigidBody* rigidBody = this->physicsWorld->createRigidBody(rigidBodyTransform);

		// returns a RigidBody wrapper
		auto wrappedRigidBody = new RigidBody{ &this->physicsCommon, rigidBody };
		this->rigidBodies.push_back(wrappedRigidBody);
		return wrappedRigidBody;
	}

	void PhysicsWorld::destroyRigidBody(RigidBody* toBeDestroyedRigidBody) {
		this->physicsWorld->destroyRigidBody(toBeDestroyedRigidBody->rigidBodyPtr);

		int index = 0;
		bool rigidBodyFound = false;

		for (RigidBody* rigidBody : this->rigidBodies) {
			if (rigidBody == toBeDestroyedRigidBody) {
				rigidBodyFound = true;
				break;
			}
			index++;
		}

		if (rigidBodyFound) {
			delete toBeDestroyedRigidBody;
			this->rigidBodies.erase(this->rigidBodies.begin() + index);
		}
	}

}