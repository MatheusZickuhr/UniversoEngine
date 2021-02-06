
#include "Engine.h"
#include "ExampleLevel.h"


int main() {
	engine::Engine engine(800, 600, "Universo Engine");
	engine.getLevelLoadingManager()->loadLevel<ExampleLevel>();
	engine.run();
	return 0;
}
