
#include <UniversoEngine.h>
#include "fps_scene/FpsScene.h"
#include "example_scene/ExampleScene.h"

int main() {	
	using namespace engine;

	Application::getInstance().initiliaze<ExampleScene>(800, 600, "Example");
	Application::getInstance().run();
	
	return 0;
}
