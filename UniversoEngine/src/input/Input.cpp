#include "Input.h"
#include "Input.h"
#include <GLFW/glfw3.h>

namespace engine {

	void Input::init(GLFWwindow* windowInstance) {
		window = windowInstance;
	}

	void Input::getCursorPos(float* xpos, float* ypos) {
		double xposAsdouble, yposAsDouble;
		glfwGetCursorPos(window, &xposAsdouble, &yposAsDouble);
		*xpos = (float)xposAsdouble;
		*ypos = (float)yposAsDouble;
	}

	bool Input::keyPressed(char key) {
		return glfwGetKey(window, key) == GLFW_PRESS;
	}

	bool Input::mouseButtonPressed(char key) {
		return glfwGetMouseButton(window, key) == GLFW_PRESS;
	}

	GLFWwindow* Input::window = nullptr;
}

