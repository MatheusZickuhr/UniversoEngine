#version 460 core

out vec4 FragColor;

in vec2 vTextureCoords;
in float vTextureSlot;

uniform sampler2D TextureSlots[32];

void main() {
    vec4 texColor = texture(TextureSlots[int(vTextureSlot)], vTextureCoords);
    FragColor = texColor;
} 
