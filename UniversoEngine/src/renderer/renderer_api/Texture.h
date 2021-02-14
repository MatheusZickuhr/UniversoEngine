#pragma once

namespace engine {

    class Texture {

    private:

        unsigned int id, slot;
        int width, height, bitsPerPixel;

        static unsigned int currentAvailableTextureSlot;
    public:


        Texture(const std::string& filepath) {

            this->slot = currentAvailableTextureSlot;
            currentAvailableTextureSlot++;

            stbi_set_flip_vertically_on_load(1);

            unsigned char* localBuffer = stbi_load(filepath.c_str(), &this->width, &this->height, &this->bitsPerPixel, 4);

            glGenTextures(1, &this->id);
            glActiveTexture(GL_TEXTURE0 + this->slot);
            glBindTexture(GL_TEXTURE_2D, this->id);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
            glBindTexture(GL_TEXTURE_2D, 0);

            if (localBuffer)
                stbi_image_free(localBuffer);
        }

        ~Texture() {
            glDeleteTextures(1, &this->id);
        }

        void bind() {
            glActiveTexture(GL_TEXTURE0 + this->slot);
            glBindTexture(GL_TEXTURE_2D, this->id);
        }

        void unbind() const {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        inline unsigned int getSlot() { return this->slot; }

        inline int getWidth() const { return width; }

        inline int getHeight() const { return height; };

    };


    unsigned int Texture::currentAvailableTextureSlot = 0;
}
