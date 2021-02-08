#pragma once

namespace engine {

	class GameObject {
	
	public:

		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Texture> texture;
		std::shared_ptr<Transform> transform;

		GameObject(
			std::shared_ptr<Mesh> mesh,
			std::shared_ptr<Texture> texture,
			std::shared_ptr<Transform> transform = std::make_shared<Transform>()
		)
		: mesh(mesh), texture(texture), transform(transform) {}

		virtual void onStart() = 0;
		virtual void onUpdate(float deltaTime) = 0;

		bool shouldRender() {
			return this->mesh != nullptr;
		} 
	};
}
