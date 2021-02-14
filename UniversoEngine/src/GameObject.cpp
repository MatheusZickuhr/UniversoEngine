#include "GameObject.h"

namespace engine {

	GameObject::GameObject(
		std::shared_ptr<Mesh> mesh,
		std::shared_ptr<Texture> texture,
		std::shared_ptr<Transform> transform
	)
		: mesh(mesh), texture(texture), transform(transform) {
	}

	bool GameObject::shouldRender() {
		return this->mesh != nullptr;
	}

}