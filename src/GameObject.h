#pragma once

namespace engine {

	class GameObject {
	
	public:

		Mesh* mesh;
		Texture* texture;
		glm::vec3 position;
		float scale;

		GameObject(Mesh* mesh, Texture* texture, float scale, glm::vec3 position)
			: mesh(mesh), texture(texture), scale(scale), position(position) {}

		GameObject() {}

		void onStart();
		void onUpdate(float deltaTime);
	};
}
