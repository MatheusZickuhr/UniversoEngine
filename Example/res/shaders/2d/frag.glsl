#version 460 core

out vec4 FragColor;

in vec2 vTextureCoords;
in float vTextureSlot;

uniform sampler2D TextureSlots[32];

void main() {
    float depthValue = texture(TextureSlots[int(vTextureSlot)], vTextureCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0);
} 
