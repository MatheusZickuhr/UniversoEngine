#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in float textureSlotIndex;

layout (location = 0) out VsOut {
    vec2 textureCoords;
    float textureSlotIndex;
} vsOut;

layout (std140, binding = 0) uniform camera {
    mat4 cameraViewProjection;
};

void main() {
    vsOut.textureCoords      = textureCoords;
    vsOut.textureSlotIndex   = textureSlotIndex;
    gl_Position = cameraViewProjection * vec4(position, 1.0);
}