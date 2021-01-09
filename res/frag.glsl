#version 330 core
out vec4 FragColor;

in vec2 vTextureCoords;

uniform vec4 Color;
uniform sampler2D TextureSlot;

void main()
{
    vec4 texColor = texture(TextureSlot, vTextureCoords);
    FragColor = texColor;
} 
