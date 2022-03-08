#pragma once

#define SCAPE_KEY = GLFW_KEY_ESCAPE


struct GLFWwindow;

namespace engine {
	
	class Input {

	private:

		static GLFWwindow* window;

	public:

		static void init(GLFWwindow* windowInstance);

		static void getCursorPos(float* xpos, float* ypos);

		static bool keyPressed(char key);

		static bool mouseButtonPressed(char key);

	};

} 