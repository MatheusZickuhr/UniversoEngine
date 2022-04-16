#include "CollisionCallback.h"

#include "../debug/Assert.h"

namespace engine {

	void CollisionCallback::onOverlap(reactphysics3d::OverlapCallback::CallbackData& callbackData) {

		for (uint32_t i = 0; i < callbackData.getNbOverlappingPairs(); i++) {

			reactphysics3d::OverlapCallback::OverlapPair overlapPair = callbackData.getOverlappingPair(i);

			reactphysics3d::CollisionBody* body1 = overlapPair.getBody1();
			reactphysics3d::CollisionBody* body2 = overlapPair.getBody2();

			reactphysics3d::CollisionBody* otherBody = body1 != targetCollisionBody ? body1 : body2;

			ASSERT(otherBody != targetCollisionBody, "OtherBody must be different from targetRigidBody");

			for (std::shared_ptr<CollisionBody> physicsWorldCollisionBody : physicsWorldCollisionBodies) {

				if (physicsWorldCollisionBody->reactphysics3CollisionBody == targetCollisionBody) {
					continue;
				}

				if (physicsWorldCollisionBody->reactphysics3CollisionBody == otherBody) {
					collidingCollisionBodies.push_back(physicsWorldCollisionBody);
					break;
				}
			}
		}
	}
}