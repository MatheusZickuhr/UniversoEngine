
#include <UniversoEngine.h>
#include "fps_scene/FpsScene.h"
#include "example_scene/ExampleScene.h"

int main() {	
	using namespace engine;

	Application app;

	app.initiliaze<ExampleScene>(800, 600, "Example");
	app.run();
	
	return 0;
}
