
#include "Engine.h"
#include "fps_scene/FpsScene.h"
#include "example_scene/ExampleScene.h"

int main() {	
	Engine::getInstance().initiliaze<ExampleScene>(800, 600, "Example");
	Engine::getInstance().run();
	
	return 0;
}
