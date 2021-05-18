#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

out vec4 FragPos; // FragPos from GS (output per emitvertex)

layout (std140, binding = 2) uniform currentPointlLight {
    mat4 shadowMatrices[6];
    vec4 lightPosition;
    float farPlane;
};

void main() {

    for(int face = 0; face < 6; ++face) {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        // for each triangle vertex
        for(int i = 0; i < 3; ++i)  {
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}  