#include "CubeMap.h"

#include <stb_image/stb_image.h>
#include <glad/glad.h>

#include "../../debug/Assert.h"

namespace engine {


    CubeMap::CubeMap() : id(0), slot(0), width(0), height(0) {
    }
	
    CubeMap::~CubeMap() {
        glDeleteTextures(1, &this->id);
    }

    std::shared_ptr<CubeMap> CubeMap::createCubeMapFromFile(const std::vector<std::string>& filepaths) {
        uint32_t cubeMapId = 0;
        
        glGenTextures(1, &cubeMapId);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);

        stbi_set_flip_vertically_on_load(0);

        int32_t width = 0;
        int32_t height = 0;
        int32_t channelsInFile = 0;
        
        for (size_t i = 0; i < filepaths.size(); i++) {

            ASSERT_FILE_EXISTS(filepaths[i]);
            ASSERT_FILE_EXTENSION(filepaths[i], { ".png", ".jpg" });

            unsigned char* data = stbi_load(filepaths[i].c_str(), &width, &height, &channelsInFile, 0);


            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + (GLenum)i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);

        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        std::shared_ptr<CubeMap> cubeMap = std::make_shared<CubeMap>();
        cubeMap->id = cubeMapId;
        cubeMap->width = width;
        cubeMap->height = height;

        return cubeMap;
    }

    uint32_t CubeMap::getId() {
        return this->id;
    }
    
    std::shared_ptr<CubeMap> CubeMap::createDepthCubeMap(int32_t width, int32_t height) {
        uint32_t cubeMapId = 0;

        glGenTextures(1, &cubeMapId);

        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);

        for (uint32_t i = 0; i < 6; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        std::shared_ptr<CubeMap> cubeMap = std::make_shared<CubeMap>();
        cubeMap->id = cubeMapId;
        cubeMap->width = width;
        cubeMap->height = height;

        return cubeMap;
    }

    int32_t CubeMap::getWidth() {
        return width;
    }

    uint32_t CubeMap::getSlot() {
        return slot;
    }

    int32_t CubeMap::getHeight() {
        return height;
    }

    void CubeMap::bind(uint32_t slot) {
        this->slot = slot;
        glActiveTexture(GL_TEXTURE0 + this->slot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);
    }

    void CubeMap::unbind() const {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
}
