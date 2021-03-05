#include "GameObject.h"

namespace engine {

	GameObject::GameObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture)
	 : mesh(mesh), texture(texture) {
			this->transform = std::make_shared<Transform>();
		}

	bool GameObject::shouldRender() {
		return this->mesh != nullptr;
	}

}