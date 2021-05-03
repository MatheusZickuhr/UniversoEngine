#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 ambient;
layout (location = 3) in vec3 diffuse;
layout (location = 4) in vec3 specular;
layout (location = 5) in float shininess;
layout (location = 6) in vec2 textureCoords;
layout (location = 7) in float textureSlot;

out vec3 vNormal;
out vec3 vAmbient;
out vec3 vDiffuse;
out vec3 vSpecular;
out float vShininess;
out vec2 vTextureCoords;
out float vTextureSlot;
out vec3 vFragPosition;

uniform mat4 viewProjection;

void main() {
    
    vNormal        = normal;
    vAmbient       = ambient;
    vDiffuse       = diffuse;
    vSpecular      = specular;
    vShininess     = shininess;
    vTextureCoords = textureCoords;
    vTextureSlot   = textureSlot;
    vFragPosition  = position;

    gl_Position = viewProjection * vec4(position, 1.0);
}