#version 440 core
out vec4 FragColor;

in vec2 vTextureCoords;
in float vTextureSlot;

uniform vec4 Color;
uniform sampler2D TextureSlot;
uniform sampler2D TextureSlots[32];

void main()
{
    // workarround, TextureSlots does not accept dynamic index
    int index = int(vTextureSlot);
    
    for (int i = 0; i < 32; i++) {
        if (index == i ) {
            index = i;
            break;
        }
    }

    vec4 texColor = texture(TextureSlots[index], vTextureCoords);
    
    FragColor = texColor;
} 
