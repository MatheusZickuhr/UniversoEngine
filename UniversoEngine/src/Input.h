#pragma once

#define SCAPE_KEY = GLFW_KEY_ESCAPE

namespace engine {
	
	class Input {

	private:

		static GLFWwindow* window;

	public:

		static void init(GLFWwindow* windowInstance) {
			window = windowInstance;
		}

		static void getCursorPos(double* xpos, double* ypos) {
			glfwGetCursorPos(window, xpos, ypos);
		}

		static bool keyPressed(char key) {
			return glfwGetKey(window, key) == GLFW_PRESS;
		} 

	};

	GLFWwindow* Input::window = nullptr;
} 