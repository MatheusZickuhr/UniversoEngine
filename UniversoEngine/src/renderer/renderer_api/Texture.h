#pragma once

#include <string>

namespace engine {

    class Texture {

    public:

        const static unsigned int maxTextureSlot = 32;

        Texture(const std::string& filepath);

        Texture(const float width, const float height);

        ~Texture();

        void bind();

        void unbind() const;

        unsigned int getSlot();
        
        unsigned int getId();

        int getWidth();

        int getHeight();

    private:

        unsigned int id, slot;
        int width, height, bitsPerPixel;

        static unsigned int currentTextureSlot;
        
    };

}
