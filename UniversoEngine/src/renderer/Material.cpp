#include "Material.h"


namespace engine {

	Material::Material()  {}

	Material::Material(glm::vec3 color) : color(color) {}

	Material::Material(const std::string& textureFilePath): texture(new Texture(textureFilePath)) {}

	Material::Material(glm::vec3 color, const std::string& textureFilePath) : 
		color(color),
		texture(new Texture(textureFilePath)) {}

	Material::~Material() {
		if (texture != nullptr) delete this->texture;
	}

	void Material::setColor(glm::vec3 color) {
		this->color = color;
	}

	void Material::setTexture(const std::string& textureFilePath) {
		this->texture = new Texture(textureFilePath);
	}

	Texture* Material::getTexture() { return this->texture; }

	glm::vec3 Material::getColor() { return this->color; }

}
