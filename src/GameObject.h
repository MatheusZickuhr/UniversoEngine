#pragma once

namespace engine {

	class GameObject {
	
	public:

		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Texture> texture;
		glm::vec3 position;
		float scale;

		GameObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture, float scale, glm::vec3 position)
			: mesh(mesh), texture(texture), scale(scale), position(position) {}

		GameObject(): scale(1.0f), position(glm::vec3(0.0f, 0.0f, 0.0f)) {}

		GameObject(const GameObject &other)
			: mesh(other.mesh), texture(other.texture), scale(other.scale), position(other.position) {}

		virtual void onStart() = 0;
		virtual void onUpdate(float deltaTime) = 0;

		bool shouldRender() {
			return this->mesh != nullptr;
		} 
	};
}
