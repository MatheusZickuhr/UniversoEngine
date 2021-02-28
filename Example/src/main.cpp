
#include "Engine.h"
#include "example_scene/ExampleScene.h"
#include "fps_scene/FpsScene.h"


int main() {
	FpsScene* initialScene = new FpsScene();
	engine::Engine* engine = new engine::Engine(initialScene, 800, 600, "Example");

	while (engine->isRunning()) {
		engine->tick();
	}

	delete initialScene;
	delete engine;

	return 0;
}
