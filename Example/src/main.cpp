
#include "Engine.h"
#include "fps_scene/FpsScene.h"
#include "example_scene/ExampleScene.h"

int main() {
	FpsScene* initialScene = new FpsScene();
	
	Engine::getInstance().initiliaze(initialScene, 800, 600, "Example");
	Engine::getInstance().run();
	
	delete initialScene;

	return 0;
}
