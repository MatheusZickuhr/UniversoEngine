#include "Time.h"

#include <GLFW/glfw3.h>


double engine::getCurrentTime() {
	return glfwGetTime();
}
