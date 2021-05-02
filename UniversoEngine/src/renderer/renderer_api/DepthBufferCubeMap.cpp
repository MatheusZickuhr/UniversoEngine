#include "DepthBufferCubeMap.h"
#include <glad/glad.h>

namespace engine {

	DepthBufferCubeMap::DepthBufferCubeMap(const float width, const float height) {
		this->width = width;
		this->height = height;

		glGenTextures(1, &this->id);

		glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);

		for (unsigned int i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
				width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	void DepthBufferCubeMap::bind(unsigned int slot) {
		this->slot = slot;
		glActiveTexture(GL_TEXTURE0 + this->slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);
	}

	void DepthBufferCubeMap::unbind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

}