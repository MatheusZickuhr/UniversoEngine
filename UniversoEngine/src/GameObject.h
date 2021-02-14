#pragma once
#include <memory>
#include "renderer/Mesh.h"
#include "renderer/renderer_api/Texture.h"
#include "Transform.h"

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
		);

		virtual void onStart() = 0;
		
		virtual void onUpdate(float deltaTime) = 0;

		bool shouldRender();
	};
}
