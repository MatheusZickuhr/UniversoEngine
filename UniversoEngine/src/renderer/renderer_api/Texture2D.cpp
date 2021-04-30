#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include <glad/glad.h>

#include "Texture2D.h"

#include "../../debug/Assert.h"

namespace engine {

	Texture2D::Texture2D(const std::string& filepath) {

		ASSERT_FILE_EXISTS(filepath);

		ASSERT_FILE_EXTENSION(filepath, { ".png", ".jpg" });

		stbi_set_flip_vertically_on_load(1);

		unsigned char* localBuffer = stbi_load(filepath.c_str(), &this->width, &this->height, &this->bitsPerPixel, 4);

		glGenTextures(1, &this->id);
		glBindTexture(GL_TEXTURE_2D, this->id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);

		if (localBuffer)
			stbi_image_free(localBuffer);
	}

	void Texture2D::bind(unsigned int slot) {
		this->slot = slot;
		glActiveTexture(GL_TEXTURE0 + this->slot);
		glBindTexture(GL_TEXTURE_2D, this->id);
	}

	void Texture2D::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
}
