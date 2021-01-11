#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

class Texture {

private:

    unsigned int id;
    int width, height, bitsPerPixel;

public:


    Texture(const std::string &filepath) {

        stbi_set_flip_vertically_on_load(1);

        unsigned char* localBuffer = stbi_load(filepath.c_str(), &width, &height, &bitsPerPixel, 4);

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (localBuffer)
            stbi_image_free(localBuffer);
    }

    ~Texture() {
        glDeleteTextures(1, &this->id);
    }

    void bind(unsigned int slot = 0) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, this->id);
    }

    void unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    inline int getWidth() const { return width; }

    inline int getHeight() const { return height; };

};