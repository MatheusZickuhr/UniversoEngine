#pragma once

#include <string>

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
