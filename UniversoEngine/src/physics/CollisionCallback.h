#pragma once
#include "CollisionBody.h"
#include <vector>
#pragma warning(push, 0)        
#include <reactphysics3d/reactphysics3d.h>
#pragma warning(pop)

namespace engine {

	class CollisionCallback : public reactphysics3d::OverlapCallback {

	public:

		CollisionCallback(std::vector<std::shared_ptr<CollisionBody>>& physicsWorldCollisionBodies, reactphysics3d::CollisionBody* targetCollisionBody)
			: physicsWorldCollisionBodies(physicsWorldCollisionBodies), targetCollisionBody(targetCollisionBody) {

		}

		virtual void onOverlap(reactphysics3d::OverlapCallback::CallbackData& callbackData) override;

		std::vector<std::shared_ptr<CollisionBody>> getCollidingRigidBodies() {
			return collidingCollisionBodies;
		}

	private:
		std::vector<std::shared_ptr<CollisionBody>> collidingCollisionBodies;
		std::vector<std::shared_ptr<CollisionBody>>& physicsWorldCollisionBodies;

		reactphysics3d::CollisionBody* targetCollisionBody;

	};
}