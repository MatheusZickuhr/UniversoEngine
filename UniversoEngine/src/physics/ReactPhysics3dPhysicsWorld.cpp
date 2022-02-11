#include "ReactPhysics3dPhysicsWorld.h"
#include "ReactPhysics3dRigidBody.h"
#include "../debug/Assert.h"

namespace engine {

	ReactPhysics3dPhysicsWorld::~ReactPhysics3dPhysicsWorld() {
		for (auto rigidBody : this->rigidBodies) {
			delete rigidBody;
		}
		this->rigidBodies.clear();
	}

	ReactPhysics3dPhysicsWorld::ReactPhysics3dPhysicsWorld() {
		this->physicsWorld = physicsCommon.createPhysicsWorld();
	}

	void ReactPhysics3dPhysicsWorld::update(float deltaTime) {
		this->physicsWorld->update(deltaTime);
	}

	RigidBody* ReactPhysics3dPhysicsWorld::createRigidBody(const glm::vec3& position, const glm::vec3& rotation) {
		//create the physics engine RigidBody
		auto rotationAsQuaternion = reactphysics3d::Quaternion::fromEulerAngles({ rotation.x, rotation.y, rotation.z });
		reactphysics3d::Transform rigidBodyTransform({ position.x, position.y, position.z }, rotationAsQuaternion);
		reactphysics3d::RigidBody* rigidBody = this->physicsWorld->createRigidBody(rigidBodyTransform);

		// returns a RigidBody wrapper
		auto wrappedRigidBody = new ReactPhysics3dRigidBody{ &this->physicsCommon, rigidBody };
		this->rigidBodies.push_back(wrappedRigidBody);
		return wrappedRigidBody;
	}

	void ReactPhysics3dPhysicsWorld::destroyRigidBody(RigidBody* toBeDestroyedRigidBody) {
		ReactPhysics3dRigidBody* reactPhysics3dRigidBody = (ReactPhysics3dRigidBody*) toBeDestroyedRigidBody;
		
		this->physicsWorld->destroyRigidBody(reactPhysics3dRigidBody->rigidBodyPtr);
		
		int index = 0;
		bool rigidBodyFound = false;

		for (RigidBody* rigidBody : this->rigidBodies) {
			if (rigidBody == reactPhysics3dRigidBody) {
				rigidBodyFound = true;
				break;
			}
			index++;
		}

		if (rigidBodyFound) {
			delete reactPhysics3dRigidBody;
			this->rigidBodies.erase(this->rigidBodies.begin() + index);
		}
	}

}
