#include "GameObject.h"

namespace engine {

	GameObject::GameObject(
		std::shared_ptr<Mesh> mesh,
		std::shared_ptr<Texture> texture	)
		: mesh(mesh), texture(texture) {
			transform = std::make_shared<Transform>();
			rigidBody = std::make_shared<RigidBody>();
	}

	bool GameObject::shouldRender() {
		return this->mesh != nullptr;
	}

}