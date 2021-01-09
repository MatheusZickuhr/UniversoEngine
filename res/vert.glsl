#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 textureCoords;

out vec2 vTextureCoords;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Scale;

void main()
{
    mat4 Mvp = Projection * View * Model;
    vec3 newPos = aPos * Scale;
    gl_Position = Mvp * vec4(newPos, 1.0);
    vTextureCoords = textureCoords;
}