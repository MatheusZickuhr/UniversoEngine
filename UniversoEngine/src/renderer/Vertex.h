#pragma once

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 normal = { 0.0f, 0.0f, 0.0f };;
    glm::vec3 ambient = { 0.0f, 0.0f, 0.0f };;
    glm::vec3 diffuse = { 0.0f, 0.0f, 0.0f };;
    glm::vec3 specular = { 0.0f, 0.0f, 0.0f };;
    float shininess = 0.0f;
    glm::vec2 textureCoords{ 0.0f, 0.0f };;
    float textureSlotIndex = -1.0f;
};