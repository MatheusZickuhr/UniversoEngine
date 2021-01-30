
#include "Engine.h"
#include "example/ExampleLevel.h"


int main() {
	engine::Engine engine(800, 600, "Universo Engine");
	engine.loadLevel<ExampleLevel>();
	engine.run();
	return 0;
}



