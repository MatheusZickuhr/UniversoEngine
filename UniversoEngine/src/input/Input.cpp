#include "Input.h"
#include <GLFW/glfw3.h>

namespace engine {

	void Input::init(GLFWwindow* windowInstance) {
		window = windowInstance;
	}

	void Input::getCursorPos(double* xpos, double* ypos) {
		glfwGetCursorPos(window, xpos, ypos);
	}

	bool Input::keyPressed(char key) {
		return glfwGetKey(window, key) == GLFW_PRESS;
	}

	GLFWwindow* Input::window = nullptr;
}

