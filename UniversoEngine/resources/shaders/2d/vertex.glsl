#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in float textureSlot;

out vec2 vTextureCoords;
out float vTextureSlot;

uniform mat4 viewProjection;

void main() {
    vTextureCoords = textureCoords;
    vTextureSlot = textureSlot;
    gl_Position = viewProjection * vec4(position, 1.0);
}