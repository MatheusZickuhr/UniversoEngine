#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in float textureSlotIndex;

out VsOut {
    vec2 textureCoords;
    float textureSlotIndex;
} vsOut;

uniform mat4 viewProjection;

void main() {
    vsOut.textureCoords      = textureCoords;
    vsOut.textureSlotIndex   = textureSlotIndex;
    gl_Position = viewProjection * vec4(position, 1.0);
}