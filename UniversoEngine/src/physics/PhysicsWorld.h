#pragma once

#include <glm/glm.hpp>
#include "RigidBody.h"
#include "CollisionBody.h"
#include <vector>
#pragma warning(push, 0)        
#include <reactphysics3d/reactphysics3d.h>
#pragma warning(pop)

#include "../debug/Log.h"
#include "../debug/Assert.h"

namespace engine {




	class PhysicsWorld {
	
	private:


		reactphysics3d::PhysicsCommon physicsCommon;
		reactphysics3d::PhysicsWorld* physicsWorld;
		std::vector<std::shared_ptr<RigidBody>> rigidBodies;
		std::vector<std::shared_ptr<CollisionBody>> collisionBodies;

	public:

		~PhysicsWorld();

		PhysicsWorld();

		void update(float deltaTime);

		std::shared_ptr<RigidBody> createRigidBody(const Transform& transform);

		std::shared_ptr<CollisionBody> createCollisionBody(const Transform& transform);

		void destroyRigidBody(std::shared_ptr<RigidBody> rigidBody);

		void destroyCollisionBody(std::shared_ptr<CollisionBody> rigidBody);

		std::vector<std::shared_ptr<CollisionBody>> getCollidingCollisionBodies(std::shared_ptr<CollisionBody> rigidBody);
	};

}