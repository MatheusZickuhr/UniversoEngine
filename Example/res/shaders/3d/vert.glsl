#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 textureCoords;
layout (location = 4) in float textureSlot;

out vec3 vColor;
out vec3 vNormal;
out vec2 vTextureCoords;
out float vTextureSlot;
out vec3 vFragPosition;

uniform mat4 Mvp;

void main() {
    vColor = color;
    vNormal = normal;
    vTextureCoords = textureCoords;
    vTextureSlot = textureSlot;

    // probabily wrong, i will fix it later
    vFragPosition =  position;

    gl_Position = Mvp * vec4(position, 1.0);
}