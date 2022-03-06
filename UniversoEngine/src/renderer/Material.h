#pragma once

#include <string>
#include <glm/glm.hpp>
#include "renderer_api/Texture.h"
#include <memory>

namespace engine {

	class Material {

	public:

		glm::vec3 ambient = { 1.0f, 1.0f, 1.0f };
		glm::vec3 diffuse = { 1.0f, 1.0f, 1.0f };
		glm::vec3 specular = { 0.5f, 0.5f, 0.5f };
		float shininess = 32.0f;

		Material();

		Material(const std::string& textureFilePath);

		void setTexture(const std::string& textureFilePath);

		std::shared_ptr<Texture> getTexture();

	private:

		std::shared_ptr<Texture> texture;

	};
}

