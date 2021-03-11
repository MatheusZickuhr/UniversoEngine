#include "PhysicsWorld.h"

namespace engine {

    void PhysicsWorld::update(float deltaTime) {

        for (auto rigidBody : this->rigidBodies) {
            if (rigidBody->isStatic) 
                continue;
       
            rigidBody->addForce(gravityForce * deltaTime);
            
            for (auto toBeAppliedForce : rigidBody->toBeAppliedForces) {
                rigidBody->applyForce(toBeAppliedForce);
                rigidBody->moveToNextState();
                int collisionCount = this->getCollisionCount(rigidBody);
                rigidBody->moveToPrevState();
                if (collisionCount > 0) {
                    rigidBody->applyForce(-toBeAppliedForce);  
                }
                
            }

            rigidBody->update();
            rigidBody->toBeAppliedForces.clear();
        }
    }

     int PhysicsWorld::getCollisionCount(RigidBody* targetRigidBody) {
        int collisionCount = 0;

        for (auto otherRigidBody : this->rigidBodies) {
           if (otherRigidBody == targetRigidBody) continue;

            bool didCollid = targetRigidBody->collidesWith(otherRigidBody);

            if (didCollid)
                collisionCount++;
            
        }

        return collisionCount;
    }

    void PhysicsWorld::appendRigidBody(RigidBody* rigidBody) {
        this->rigidBodies.push_back(rigidBody);
    }

    void PhysicsWorld::clear() {
        this->rigidBodies.clear();
    }
}