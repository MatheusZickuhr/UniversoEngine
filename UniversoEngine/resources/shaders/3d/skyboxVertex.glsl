#version 460 core

layout (location = 0) in vec3 position;

layout (location = 0) out vec3 texCoords;

layout (std140, binding = 0) uniform camera {
    mat4 cameraViewProjection;
    mat4 cameraView;
    mat4 cameraProjection;
    vec3 cameraPosition;
};

void main() {
    texCoords = position;

    mat4 changedCameraView = mat4(mat3(cameraView));
    
    vec4 pos = cameraProjection * changedCameraView *  vec4(position, 1.0);
    gl_Position = pos.xyww;
}  