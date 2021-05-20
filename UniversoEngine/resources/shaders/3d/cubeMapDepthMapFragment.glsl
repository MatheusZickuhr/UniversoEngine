#version 460 core

layout (location = 0) in vec4 FragPos;

layout (std140, binding = 2) uniform currentPointlLight {
    mat4 shadowMatrices[6];
    vec4 lightPosition;
    float farPlane;
};

void main()
{
    // get distance between fragment and light source
    float lightDistance = length(FragPos.xyz - lightPosition.xyz);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / farPlane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}  