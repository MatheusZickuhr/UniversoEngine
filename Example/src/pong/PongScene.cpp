#include "PongScene.h"

void PongScene::onStart() {

	this->camera3d->position = { 0.0f, 0.0f, 0.0f };

	cameraController = std::make_unique<Camera3dController>(this->window, this->camera3d);

	

	{
		sprite = createEntity();
		sprite.addComponent<TransformComponent>();
		auto& boxTransformComponent = sprite.getComponent<TransformComponent>();
		boxTransformComponent.transform.position = { 0.0f, 0.0f, 0.0f };
		//boxTransformComponent.transform.scale = { 100.0f, 100.0f, 1.0f };
		std::shared_ptr<Texture> texture = Texture::createTextureFromFile("Example/resources/textures/crate/crate.jpg");
		sprite.addComponent<SpriteComponent>(texture);

	}
}

void PongScene::onUpdate(float deltaTime) {
	cameraController->update(deltaTime);

	//auto& boxTransformComponent = sprite.getComponent<TransformComponent>();
	//boxTransformComponent.transform.position.x += 0.1f * deltaTime;
}
