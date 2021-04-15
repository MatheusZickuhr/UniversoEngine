#pragma once

#include <string>
#include <glm/glm.hpp>
#include "renderer_api/Texture.h"

namespace engine {

	class Material {

	public:

		Material();

		Material(glm::vec3 color);

		Material(const std::string& textureFilePath);

		Material(glm::vec3 color, const std::string& textureFilePath);

		~Material();

		void setColor(glm::vec3 color);

		void setTexture(const std::string& textureFilePath);

		Texture* getTexture();

		glm::vec3 getColor();

	private:

		glm::vec3 color = { 1.0f, 1.0f, 1.0f };
		Texture* texture = nullptr;

	};
}

