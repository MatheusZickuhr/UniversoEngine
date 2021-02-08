#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in float textureSlot;
layout (location = 3) in float rotationAngle;
layout (location = 4) in vec3 rotationAxis;

out vec2 vTextureCoords;
out float vTextureSlot;

uniform mat4 Mvp;


vec4 quaternionFromAxisAngle(vec3 axis, float angle) { 
    vec4 qr;
    float half_angle = (angle * 0.5) * 3.14159 / 180.0;
    qr.x = axis.x * sin(half_angle);
    qr.y = axis.y * sin(half_angle);
    qr.z = axis.z * sin(half_angle);
    qr.w = cos(half_angle);
    return qr;
}


vec3 rotateVertexPosition(vec3 position, vec3 axis, float angle) { 
    vec4 q = quaternionFromAxisAngle(axis, angle);
    vec3 v = position.xyz;
    return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v);
}

void main() {
    vTextureCoords = textureCoords;
    vTextureSlot = textureSlot;
    vec3 rotatedPos = rotateVertexPosition(aPos, rotationAxis, rotationAngle);
    gl_Position = Mvp * vec4(rotatedPos, 1.0);
}