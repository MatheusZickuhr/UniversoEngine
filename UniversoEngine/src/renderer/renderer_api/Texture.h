#pragma once

#include <string>

namespace engine {

    class Texture {

    public:

        const static unsigned int maxTextureSlot = 32;

        Texture(const std::string& filepath);

        ~Texture();

        void bind();

        void unbind() const;

        unsigned int getSlot();

        int getWidth();

        int getHeight();

    private:

        unsigned int id, slot;
        int width, height, bitsPerPixel;

        static unsigned int currentTextureSlot;
        
    };

}
