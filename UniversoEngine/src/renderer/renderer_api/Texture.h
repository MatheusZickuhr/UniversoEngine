#pragma once

#include <memory>
#include <string>

namespace engine {

    class Texture {

    public:

        const static unsigned int MAX_TEXTURES = 16;
        
        Texture();
        
        Texture(const Texture& other) = delete;

        ~Texture();

        void bind(unsigned int slot);

        void unbind() const;

        unsigned int getSlot();
        
        unsigned int getId();

        int getWidth();

        int getHeight();

        static std::shared_ptr<Texture> createTextureFromFile(const std::string& filePath);
        
        static std::shared_ptr<Texture> createDepthTexture(int width, int height);
        
    private:

        unsigned int id, slot = 0;
        int width, height = 0;

        
    };

}
