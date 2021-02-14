#pragma once

#define SCAPE_KEY = GLFW_KEY_ESCAPE

#include <GLFW/glfw3.h>

namespace engine {
	
	class Input {

	private:

		static GLFWwindow* window;

	public:

		static void init(GLFWwindow* windowInstance);

		static void getCursorPos(double* xpos, double* ypos);

		static bool keyPressed(char key);

	};

} 