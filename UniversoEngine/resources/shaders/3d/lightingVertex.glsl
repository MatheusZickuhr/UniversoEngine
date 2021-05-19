#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 ambient;
layout (location = 3) in vec3 diffuse;
layout (location = 4) in vec3 specular;
layout (location = 5) in float shininess;
layout (location = 6) in vec2 textureCoords;
layout (location = 7) in float textureSlotIndex;

out VsOut {
    vec3 normal;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    vec2 textureCoords;
    float textureSlotIndex;
    vec3 fragPosition;
} vsOut;


layout (std140, binding = 0) uniform camera {
    mat4 cameraViewProjection;
    vec3 cameraPosition;
};

void main() {
    
    vsOut.normal             = normal;
    vsOut.ambient            = ambient;
    vsOut.diffuse            = diffuse;
    vsOut.specular           = specular;
    vsOut.shininess          = shininess;
    vsOut.textureCoords      = textureCoords;
    vsOut.textureSlotIndex   = textureSlotIndex;
    vsOut.fragPosition       = position;

    gl_Position = cameraViewProjection * vec4(position, 1.0);
}