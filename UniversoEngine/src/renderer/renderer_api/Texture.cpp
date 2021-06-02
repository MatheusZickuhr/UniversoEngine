#include <glad/glad.h>

#include "Texture.h"

namespace engine {

	Texture::~Texture() { glDeleteTextures(1, &this->id); }

	unsigned int Texture::getSlot() { return this->slot; }

	unsigned int Texture::getId() { return this->id; }

	int Texture::getWidth() { return width; }

	int Texture::getHeight() { return height; };

}