#include "ShipBehavior.h"
#include "ProjectileBehavior.h"

void ShipBehavior::onStart() {
}

void ShipBehavior::onUpdate(float deltaTime) {
	auto& playerTransformComponent = entity.getComponent<TransformComponent>();
	
	handleRotationMovement(deltaTime, playerTransformComponent);
	handleMovement(deltaTime, playerTransformComponent);
}

void ShipBehavior::handleRotationMovement(float deltaTime, TransformComponent& playerTransformComponent) {

	float rotationSpeed = 7.0f;

	// handle rotation
	if (Input::keyPressed(Input::KEY_N)) {
		playerTransformComponent.transform.rotation.z += rotationSpeed * deltaTime;
	}
	else if (Input::keyPressed(Input::KEY_M)) {
		playerTransformComponent.transform.rotation.z -= rotationSpeed * deltaTime;
	}
}

void ShipBehavior::handleMovement(float deltaTime, TransformComponent& playerTransformComponent) {
	float movementSpeed = 10.0f;

	if (Input::keyPressed(Input::KEY_A)) {
		playerTransformComponent.transform.position.x -= movementSpeed * deltaTime;
	}
	else if (Input::keyPressed(Input::KEY_D)) {
		playerTransformComponent.transform.position.x += movementSpeed * deltaTime;
	}
	else if (Input::keyPressed(Input::KEY_S)) {
		playerTransformComponent.transform.position.y -= movementSpeed * deltaTime;
	}
	else if (Input::keyPressed(Input::KEY_W)) {
		playerTransformComponent.transform.position.y += movementSpeed * deltaTime;
	}
}


