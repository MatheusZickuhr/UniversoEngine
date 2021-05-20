#version 460 core

layout (location = 0) out vec4 FragColor;

layout (location = 0) in VsOut {
    vec2 textureCoords;
    float textureSlotIndex;
} fsIn;

layout (binding = 0) uniform sampler2D TextureSlots[32];

void main() {
    vec4 texColor = texture(TextureSlots[int(fsIn.textureSlotIndex)], fsIn.textureCoords);
    FragColor = texColor;
} 
