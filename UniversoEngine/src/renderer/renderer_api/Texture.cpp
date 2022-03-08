
#include <stb_image/stb_image.h>
#include <glad/glad.h>
#include "Texture.h"
#include "../../debug/Assert.h"

namespace engine {
	Texture::Texture() : id(0), slot(0), width(0), height(0) {
	}

	Texture::~Texture() {
		glDeleteTextures(1, &this->id);
	}

	std::shared_ptr<Texture> Texture::createTextureFromFile(const std::string& filePath) {
		ASSERT_FILE_EXISTS(filePath);
		ASSERT_FILE_EXTENSION(filePath, { ".png", ".jpg" });

		stbi_set_flip_vertically_on_load(1);

		int32_t width, height, channelsInFile = 0;
		uint32_t textureId = 0;

		unsigned char* localBuffer = stbi_load(filePath.c_str(), &width, &height, &channelsInFile, 4);

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);

		if (localBuffer) {
			stbi_image_free(localBuffer);
		}

		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		texture->id = textureId;
		texture->width = width;
		texture->height = height;

		return texture;
	}

	std::shared_ptr<Texture> Texture::createDepthTexture(int32_t width, int32_t height) {
		uint32_t textureId = 0;

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		texture->id = textureId;
		texture->width = width;
		texture->height = height;

		return texture;
	}
	
	void Texture::bind(uint32_t slot) {
		this->slot = slot;
		glActiveTexture(GL_TEXTURE0 + this->slot);
		glBindTexture(GL_TEXTURE_2D, this->id);
	}

	void Texture::unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	uint32_t Texture::getSlot() {
		return this->slot;
	}

	uint32_t Texture::getId() {
		return this->id;
	}

	int32_t Texture::getWidth() {
		return width;
	}

	int32_t Texture::getHeight() {
		return height;
	}

}