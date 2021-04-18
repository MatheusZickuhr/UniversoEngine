#pragma once

#include <glm/glm.hpp>

struct DirectionalLight {
    glm::vec3 direction = { 0.0f, 0.0f, 0.0f };
    glm::vec3 ambient = { 0.2f, 0.2f, 0.2f };
    glm::vec3 diffuse = { 0.5f, 0.5f, 0.5f };
    glm::vec3 specular = { 1.0f, 1.0f, 1.0f };
};