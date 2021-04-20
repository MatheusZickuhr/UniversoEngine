#include "Material.h"


namespace engine {

	Material::Material()  {}

	Material::Material(const std::string& textureFilePath): texture(new Texture(textureFilePath)) {}

	Material::~Material() {
		if (texture != nullptr) delete this->texture;
	}

	void Material::setTexture(const std::string& textureFilePath) {
		this->texture = new Texture(textureFilePath);
	}

	Texture* Material::getTexture() { return this->texture; }

}
