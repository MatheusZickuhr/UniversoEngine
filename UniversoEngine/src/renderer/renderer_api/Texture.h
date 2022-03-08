#pragma once

#include <memory>
#include <string>

namespace engine {

    class Texture {

    public:

        const static uint32_t MAX_TEXTURES = 16;
        
        Texture();
        
        Texture(const Texture& other) = delete;

        ~Texture();

        void bind(uint32_t slot);

        void unbind() const;

        uint32_t getSlot();
        
        uint32_t getId();

        int32_t getWidth();

        int32_t getHeight();

        static std::shared_ptr<Texture> createTextureFromFile(const std::string& filePath);
        
        static std::shared_ptr<Texture> createDepthTexture(int32_t width, int32_t height);
        
    private:

        uint32_t id, slot;
        int32_t width, height;

        
    };

}
