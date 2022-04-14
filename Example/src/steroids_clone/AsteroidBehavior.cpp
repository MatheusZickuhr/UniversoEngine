#include "AsteroidBehavior.h"

void AsteroidBehavior::onStart() {
	speed = (random.random() * 4.0f) + 4.0f;
}

void AsteroidBehavior::onUpdate(float deltaTime) {
	currentLifeTime += deltaTime;

	auto& transformComponent = entity.getComponent<TransformComponent>();
	transformComponent.transform.position.x += direction.x * deltaTime * speed;
	transformComponent.transform.position.y += direction.y * deltaTime * speed;

	if (currentLifeTime > maxLifeTime) {
		scene->destroyEntity(entity);
		LOG("Asteroid destroyed!");
	}
}

void AsteroidBehavior::onCollision(Entity other) {

	if (other.hasComponent<NameComponent>()) {
		
		if (other.getComponent<NameComponent>().name == "asteroid") {
			this->scene->destroyEntity(entity);
		}

		else if (other.getComponent<NameComponent>().name == "player") {
			this->scene->destroyEntity(other);
		}
	}
}
