#include "CubeMap.h"

#include <stb_image/stb_image.h>
#include <glad/glad.h>

#include "../../debug/Assert.h"

namespace engine {
	
	CubeMap::CubeMap(const std::vector<std::string>& filepaths) {
        
        glGenTextures(1, &this->id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);

        stbi_set_flip_vertically_on_load(0);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < filepaths.size(); i++) {

            ASSERT_FILE_EXISTS(filepaths[i]);
            ASSERT_FILE_EXTENSION(filepaths[i], { ".png", ".jpg" });

            unsigned char* data = stbi_load(filepaths[i].c_str(), &width, &height, &nrChannels, 0);
            
            
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
            
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	void CubeMap::bind(unsigned int slot) {
        this->slot = slot;
        glActiveTexture(GL_TEXTURE0 + this->slot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);
	}

	void CubeMap::unbind() const {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}
