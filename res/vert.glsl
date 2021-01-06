#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 Mvp;

void main()
{
    gl_Position = Mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}