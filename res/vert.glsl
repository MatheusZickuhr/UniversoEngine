#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 Mvp;
uniform float Scale;

void main()
{
    vec3 newPos = aPos * Scale;
    gl_Position = Mvp * vec4(newPos, 1.0);
}