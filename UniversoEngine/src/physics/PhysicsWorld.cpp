#include "PhysicsWorld.h"

namespace engine {
	
	void CollisionCallback::onOverlap(reactphysics3d::OverlapCallback::CallbackData& callbackData) {

		for (uint32_t i = 0; i < callbackData.getNbOverlappingPairs(); i++) {
		
			reactphysics3d::OverlapCallback::OverlapPair overlapPair = callbackData.getOverlappingPair(i);

			reactphysics3d::CollisionBody* body1 = overlapPair.getBody1();
			reactphysics3d::CollisionBody* body2 = overlapPair.getBody2();
		
			reactphysics3d::CollisionBody* otherBody = body1 != targetCollisionBody ? body1 : body2;

			ASSERT(otherBody != targetCollisionBody, "OtherBody must be different from targetRigidBody");

			for (CollisionBody* physicsWorldCollisionBody : physicsWorldCollisionBodies) {

				if (physicsWorldCollisionBody->collisionBodyPtr == targetCollisionBody) {
					continue;
				}

				if (physicsWorldCollisionBody->collisionBodyPtr == otherBody) {
					collidingCollisionBodies.push_back(physicsWorldCollisionBody);
					break;
				}
			}
		}
	}

	PhysicsWorld::~PhysicsWorld() {
		for (RigidBody* rigidBody : this->rigidBodies) {
			delete rigidBody;
		}
		this->rigidBodies.clear();
		
		for (CollisionBody* collisionBody : this->collisionBodies) {
			delete collisionBody;
		}
		this->collisionBodies.clear();

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

	CollisionBody* PhysicsWorld::createCollisionBody(const glm::vec3& position, const glm::vec3& rotation) {
		//create the physics engine CollisionBody
		auto rotationAsQuaternion = reactphysics3d::Quaternion::fromEulerAngles({ rotation.x, rotation.y, rotation.z });
		reactphysics3d::Transform collisionBodyTransform({ position.x, position.y, position.z }, rotationAsQuaternion);
		reactphysics3d::CollisionBody* collisionBody = this->physicsWorld->createCollisionBody(collisionBodyTransform);

		// returns a CollisionBody wrapper
		auto wrappedCollisionBody = new CollisionBody{ &this->physicsCommon, collisionBody };
		this->collisionBodies.push_back(wrappedCollisionBody);
		return wrappedCollisionBody;
	}


	void PhysicsWorld::destroyRigidBody(RigidBody* toBeDestroyedRigidBody) {
		this->physicsWorld->destroyRigidBody(toBeDestroyedRigidBody->rigidBodyPtr);

		size_t index = 0;
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

	void PhysicsWorld::destroyCollisionBody(CollisionBody* toBeDestroyedCollisionBody) {
		this->physicsWorld->destroyCollisionBody(toBeDestroyedCollisionBody->collisionBodyPtr);

		size_t index = 0;
		bool collisionBodyFound = false;

		for (CollisionBody* collisionBody : this->collisionBodies) {
			if (collisionBody == toBeDestroyedCollisionBody) {
				collisionBodyFound = true;
				break;
			}
			index++;
		}

		if (collisionBodyFound) {
			delete toBeDestroyedCollisionBody;
			this->collisionBodies.erase(this->collisionBodies.begin() + index);
		}
	}

	// returns a vector with collision bodies colliding with the parameter collisionbody
	std::vector<CollisionBody*> PhysicsWorld::getCollidingBodies(CollisionBody* rigidBody) {
		CollisionCallback myCollisionCallback { this->collisionBodies, rigidBody->collisionBodyPtr };
		this->physicsWorld->testOverlap(rigidBody->collisionBodyPtr, myCollisionCallback);
		return myCollisionCallback.getCollidingRigidBodies();
	}

}