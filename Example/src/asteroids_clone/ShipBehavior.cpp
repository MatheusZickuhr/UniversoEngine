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


	auto window = scene->getWindow();

	// handle rotation
	if (window->keyPressed(Window::KEY_N)) {
		playerTransformComponent.transform.rotation.z += rotationSpeed * deltaTime;
	}
	else if (window->keyPressed(Window::KEY_M)) {
		playerTransformComponent.transform.rotation.z -= rotationSpeed * deltaTime;
	}
}

void ShipBehavior::handleMovement(float deltaTime, TransformComponent& playerTransformComponent) {
	float movementSpeed = 10.0f;

	auto window = scene->getWindow();

	if (window->keyPressed(Window::KEY_A)) {
		playerTransformComponent.transform.position.x -= movementSpeed * deltaTime;
	}
	else if (window->keyPressed(Window::KEY_D)) {
		playerTransformComponent.transform.position.x += movementSpeed * deltaTime;
	}
	else if (window->keyPressed(Window::KEY_S)) {
		playerTransformComponent.transform.position.y -= movementSpeed * deltaTime;
	}
	else if (window->keyPressed(Window::KEY_W)) {
		playerTransformComponent.transform.position.y += movementSpeed * deltaTime;
	}
}


