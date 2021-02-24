#pragma once
#include <memory>
#include "../renderer/Mesh.h"
#include "../renderer/renderer_api/Texture.h"
#include "../physics/RigidBody.h"
#include "Transform.h"

namespace engine {

	class GameObject {
	
	public:

		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Texture> texture;
		std::shared_ptr<Transform> transform;
		std::shared_ptr<RigidBody> rigidBody;

		GameObject(
			std::shared_ptr<Mesh> mesh,
			std::shared_ptr<Texture> texture
		);

		virtual void onStart() = 0;
		
		virtual void onUpdate(float deltaTime) = 0;

		bool shouldRender();
	};
}
