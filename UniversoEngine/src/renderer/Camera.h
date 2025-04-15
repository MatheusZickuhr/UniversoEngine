#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace engine {
	
    class Camera {

    public:

		virtual ~Camera() = default;

        virtual glm::mat4 getViewMatrix() = 0;

        virtual glm::mat4 getProjectionMatrix(float width, float height) = 0;

        virtual glm::mat4 getViewProjectionMatrix(float width, float height) = 0;

        virtual glm::vec3 getPosition() = 0;

    };
}
