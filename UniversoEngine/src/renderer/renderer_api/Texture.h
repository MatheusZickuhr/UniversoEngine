#pragma once

namespace engine {

    class Texture {

    public:

        const static unsigned int MAX_TEXTURES = 16;
        const static unsigned int MAX_CUBEMAPS = 16;
        

        ~Texture();

        virtual void bind(unsigned int slot) = 0;

        virtual void unbind() const = 0;

        unsigned int getSlot();
        
        unsigned int getId();

        int getWidth();

        int getHeight();

    protected:

        unsigned int id, slot = 0;
        int width, height = 0;
        
    };

}
