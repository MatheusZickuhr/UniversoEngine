#include "Material.h"


namespace engine {

	Material::Material()  {}

	Material::Material(const std::string& textureFilePath): 
		texture(Texture::createTextureFromFile(textureFilePath)) {}

	void Material::setTexture(const std::string& textureFilePath) {
		this->texture = Texture::createTextureFromFile(textureFilePath);
	}

	std::shared_ptr<Texture> Material::getTexture() {
		return this->texture;
	}

}
