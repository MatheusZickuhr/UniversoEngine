#include "Material.h"


namespace engine {

	Material::Material()  {}

	Material::Material(const std::string& textureFilePath): texture(new Texture2D(textureFilePath)) {}

	Material::~Material() {
		if (texture != nullptr) delete this->texture;
	}

	void Material::setTexture(const std::string& textureFilePath) {
		if (this->texture != nullptr) delete this->texture;
		this->texture = new Texture2D(textureFilePath);
	}

	Texture2D* Material::getTexture() { return this->texture; }

}
