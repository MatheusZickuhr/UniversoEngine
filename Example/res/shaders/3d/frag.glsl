#version 460 core
out vec4 FragColor;

in vec2 vTextureCoords;
in float vTextureSlot;

uniform sampler2D textureSlots[32];

void main() {
    if (vTextureSlot == -1) {
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    } else {
        FragColor = texture(textureSlots[int(vTextureSlot)], vTextureCoords);
    }
} 
