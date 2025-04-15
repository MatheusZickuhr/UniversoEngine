#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

namespace engine {
	class Camera3d : public Camera {

    public:

        glm::vec3 position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 up = { 0.0f, 0.0f, 0.0f };
        glm::vec3 right = { 0.0f, 0.0f, 0.0f };
        glm::vec3 front = { 0.0f, 0.0f, -1.0f };
        glm::vec3 worldUp = { 0.0f, 1.0f, 0.0f };

        //clipping values
        float nearClippingDistance = 0.1f;
        float farClippingDistance = 1000.0f;

        // euler Angles
        float yaw = -90.0f;
        float pitch = 0.0f;

        float fov = 45.0f;

        Camera3d();

        Camera3d(glm::vec3 position);


        glm::mat4 getViewMatrix() override;

        glm::mat4 getProjectionMatrix(float width, float height) override;

        glm::mat4 getViewProjectionMatrix(float width, float height) override;

		glm::vec3 getPosition() override { return this->position; }

    private:

        void updateVectors();
	};
}