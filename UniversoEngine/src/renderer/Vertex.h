#pragma once

struct Vertex {
    glm::vec3 position;
    glm::vec2 textureCoords;
    float textureSlot = -1.0f;
    float rotationAngle = 0.0f;
    glm::vec3 rotationAxis;
};
