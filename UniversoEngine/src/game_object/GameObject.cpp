#include "GameObject.h"

namespace engine {

	GameObject::GameObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture)
	 : mesh(mesh), texture(texture) {

			this->transform = std::make_shared<Transform>();

			// rigid body creation (with collision mesh)
			this->rigidBody = std::make_shared<RigidBody>();
			
			std::vector<glm::vec3> vertices;

			for (auto vertex : this->mesh->vertices) 
				vertices.push_back(vertex.position);
			
			this->rigidBody->collisionMesh = std::make_shared<CollisionMesh>(vertices);
		}

	bool GameObject::shouldRender() {
		return this->mesh != nullptr;
	}

}