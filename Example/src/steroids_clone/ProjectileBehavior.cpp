#include "ProjectileBehavior.h"

void ProjectileBehavior::onStart() {
}

void ProjectileBehavior::onUpdate(float deltaTime) {
	currentLifeTime += deltaTime;
		
	auto& transformComponent = entity.getComponent<TransformComponent>();
	transformComponent.transform.position.x += direction.x * deltaTime * speed;
	transformComponent.transform.position.y += direction.y * deltaTime * speed;

	if (currentLifeTime > maxLifeTime) {
		scene->destroyEntity(entity);
		LOG("Projectile destroyed!");
	}
}

void ProjectileBehavior::onCollision(Entity other) {

	if (other.hasComponent<NameComponent>() 
		&& other.getComponent<NameComponent>().name == "asteroid") {
		scene->destroyEntity(other);
	}
}
