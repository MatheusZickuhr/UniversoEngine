#pragma once

#include <string>
#include <stb_image/stb_image.h>
#include <glad/glad.h>

namespace engine {

    class Texture {

    private:

        unsigned int id, slot;
        int width, height, bitsPerPixel;

        static unsigned int currentAvailableTextureSlot;
    public:

        Texture(const std::string& filepath);

        ~Texture();

        void bind();

        void unbind() const;

        unsigned int getSlot();

        int getWidth();

        int getHeight();

    };

}
