#version 460 core

out vec4 FragColor;

in VsOut {
    vec2 textureCoords;
    float textureSlotIndex;
} fsIn;

layout (binding = 0) uniform sampler2D TextureSlots[32];

void main() {
    vec4 texColor = texture(TextureSlots[int(fsIn.textureSlotIndex)], fsIn.textureCoords);
    FragColor = texColor;
} 
