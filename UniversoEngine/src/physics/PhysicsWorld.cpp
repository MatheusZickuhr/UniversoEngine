#include "PhysicsWorld.h"
#include "CollisionCallback.h"


namespace engine {
	

	reactphysics3d::Transform createReactphysics3dTransform(const glm::vec3& position, const glm::vec3& rotation) {
		auto rotationAsQuaternion = reactphysics3d::Quaternion::fromEulerAngles({ rotation.x, rotation.y, rotation.z });
		return reactphysics3d::Transform { { position.x, position.y, position.z }, rotationAsQuaternion };
	}

	PhysicsWorld::~PhysicsWorld() {
		this->physicsCommon.destroyPhysicsWorld(this->physicsWorld);
	}

	PhysicsWorld::PhysicsWorld() {
		this->physicsWorld = physicsCommon.createPhysicsWorld();
	}

	void PhysicsWorld::update(float deltaTime) {
		this->physicsWorld->update(deltaTime);
	}

	std::shared_ptr<RigidBody> PhysicsWorld::createRigidBody(const Transform& transform) {
		reactphysics3d::RigidBody* reactphysics3dRigidBody = this->physicsWorld
			->createRigidBody(createReactphysics3dTransform(transform.position, transform.rotation));

		auto rigidBody = std::make_shared<RigidBody>(&this->physicsCommon, reactphysics3dRigidBody);
		this->rigidBodies.push_back(rigidBody);
		return rigidBody;
	}

	std::shared_ptr<CollisionBody> PhysicsWorld::createCollisionBody(const Transform& transform) {
		reactphysics3d::CollisionBody* reactphysics3CollisionBody = this->physicsWorld
			->createCollisionBody(createReactphysics3dTransform(transform.position, transform.rotation));

		
		auto collisionBody = std::make_shared<CollisionBody>(&this->physicsCommon, reactphysics3CollisionBody);
		this->collisionBodies.push_back(collisionBody);
		return collisionBody;
	}


	void PhysicsWorld::destroyRigidBody(std::shared_ptr<RigidBody>  toBeDestroyedRigidBody) {
		physicsWorld->destroyRigidBody(toBeDestroyedRigidBody->reactphysics3dRigidBody);
		rigidBodies.erase(
			std::find(
				rigidBodies.begin(),
				rigidBodies.end(),
				toBeDestroyedRigidBody
			)
		);
	}

	void PhysicsWorld::destroyCollisionBody(std::shared_ptr<CollisionBody> toBeDestroyedCollisionBody) {
		this->physicsWorld->destroyCollisionBody(toBeDestroyedCollisionBody->reactphysics3CollisionBody);
		collisionBodies.erase(
			std::find(
				collisionBodies.begin(),
				collisionBodies.end(),
				toBeDestroyedCollisionBody
			)
		);
	}

	std::vector<std::shared_ptr<CollisionBody>> PhysicsWorld::getCollidingCollisionBodies(std::shared_ptr<CollisionBody> rigidBody) {
		CollisionCallback myCollisionCallback { this->collisionBodies, rigidBody->reactphysics3CollisionBody };
		this->physicsWorld->testOverlap(rigidBody->reactphysics3CollisionBody, myCollisionCallback);
		return myCollisionCallback.getCollidingRigidBodies();
	}

}