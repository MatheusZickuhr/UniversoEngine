#define STB_IMAGE_IMPLEMENTATION

#include "Texture.h"

namespace engine {

	unsigned int Texture::currentAvailableTextureSlot = 0;

	Texture::Texture(const std::string& filepath) {

		this->slot = currentAvailableTextureSlot;
		currentAvailableTextureSlot++;

		stbi_set_flip_vertically_on_load(1);

		unsigned char* localBuffer = stbi_load(filepath.c_str(), &this->width, &this->height, &this->bitsPerPixel, 4);

		glGenTextures(1, &this->id);
		glActiveTexture(GL_TEXTURE0 + this->slot);
		glBindTexture(GL_TEXTURE_2D, this->id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (localBuffer)
			stbi_image_free(localBuffer);
	}

	Texture::~Texture() {
		glDeleteTextures(1, &this->id);
	}

	void Texture::bind() {
		glActiveTexture(GL_TEXTURE0 + this->slot);
		glBindTexture(GL_TEXTURE_2D, this->id);
	}

	void Texture::unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	unsigned int Texture::getSlot() { return this->slot; }

	int Texture::getWidth() { return width; }

	int Texture::getHeight() { return height; };

}